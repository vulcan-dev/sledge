using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;
using System.Threading;

/*
 * TO-DO: Implement exception handler
 */

namespace SledgeLib
{
    internal class ModManager
    {
        internal static FileSystemWatcher? Watcher = null;
        internal static string? BinFolderPath;
        internal static string? ModsFolder;
        internal static string? CurrentLoadingModPath = null;

        internal static Dictionary<string, ModContext> ModList = new Dictionary<string, ModContext>();

        /*
         *  loads all available mods and sets up file watchers 
         */
        internal static void Init()
        {
            /*
             * get the required paths for loading mods
             */
            BinFolderPath = Path.GetDirectoryName(typeof(ModManager).Assembly.Location);
            if (BinFolderPath == null)
                throw new System.Exception("Unable to get assembly location");

            ModsFolder = BinFolderPath + "\\..\\mods";
            if (!Directory.Exists(ModsFolder))
                Directory.CreateDirectory(ModsFolder);

            /*
             * iterate mods folder and load mods from there
             */
            string[] ModFolderContent = Directory.GetFiles(ModsFolder);
            foreach (string FilePath in ModFolderContent)
            {
                if (Path.GetExtension(FilePath) != ".dll")
                    continue;

                Log.Verbose("Loading mod: {0}", FilePath);

                ModContext ModCtx;
                try
                {
                    ModCtx = new ModContext(FilePath);
                } catch (Exception ex)
                {
                    Log.Error("Exception thrown while loading file {0} : {1}", Path.GetFileName(FilePath), ex);
                    continue;
                }

                Log.Verbose("Loaded mod: {0} by {1}", ModCtx.m_Interface.GetName(), ModCtx.m_Interface.GetAuthor());
            }

            /*
             * create watcher for handling mod loading / unloading / reloading
             */
            Watcher = new FileSystemWatcher(ModsFolder);
            Watcher.Created += OnFileCreated;
            Watcher.Changed += OnFileChanged;
            Watcher.Deleted += OnFileDeleted;
            Watcher.EnableRaisingEvents = true;
            Watcher.NotifyFilter = NotifyFilters.Attributes | NotifyFilters.DirectoryName | NotifyFilters.FileName | NotifyFilters.LastAccess | NotifyFilters.LastWrite;

            Log.General("Loaded mods and registered watcher functions");
        }

        /*
         * class for the mod's information and context
         */
        internal class ModContext : AssemblyLoadContext
        {
            internal Assembly m_Assembly;
            internal DateTime m_AssemblyLastWrite;

            internal string? m_DataFolder;
            internal string m_AssemblyName;
            internal string m_AssemblyPath;

            internal ISledgeMod m_Interface;

            public ModContext(string AssemblyPath) : base(name: AssemblyPath, isCollectible: true)
            {
                lock (ModList)
                {
                    if (CurrentLoadingModPath != null)
                        Log.Warning("CurrentLoadingModPath wasn't null.");

                    CurrentLoadingModPath = AssemblyPath;

                    m_AssemblyPath = AssemblyPath;
                    m_AssemblyName = Path.GetFileNameWithoutExtension(AssemblyPath);
                    m_AssemblyLastWrite = File.GetLastWriteTime(AssemblyPath);

                    Resolving += ModDependencyResolver;

                    /*
                     * check if mod specifies it's own folder (for data, dependencies, etc)
                     */
                    if (Directory.Exists(ModsFolder + "\\" + m_AssemblyName))
                        m_DataFolder = ModsFolder + "\\" + m_AssemblyName;

                    /*
                     * load all dependencies first
                     */
                    if (m_DataFolder != null)
                    {
                        string DependenciesFolder = String.Format("{0}\\dependencies", m_DataFolder);

                        if (Directory.Exists(DependenciesFolder)) {
                            string[] DependenciesFolderContent = Directory.GetFiles(DependenciesFolder);
                            foreach (string DependencyPath in DependenciesFolderContent)
                            {
                                try
                                {
                                    FileStream DependencyStream = File.OpenRead(DependencyPath);
                                    LoadFromStream(DependencyStream);
                                    Log.Verbose("Loaded dependency: {0}", Path.GetFileNameWithoutExtension(DependencyPath));
                                } catch (Exception ex)
                                {
                                    CurrentLoadingModPath = null;
                                    throw new Exception(string.Format("Error ocurred while loading dependency: {0} - {1}", DependencyPath, ex));
                                }
                            }
                        }
                    }

                    /*
                     * load DLL from assembly so that the assembly may be deleted or changed after it's loaded
                     */
                    int LoadAttempts = 0;
                    FileStream AssemblyStream;
                    while (true)
                    {
                        try
                        {
                            AssemblyStream = File.OpenRead(AssemblyPath);
                            break;
                        }
                        catch (Exception)
                        {
                            LoadAttempts++;
                            Thread.Sleep(250);
                            if (LoadAttempts >= 5)
                            {
                                CurrentLoadingModPath = null;
                                throw new Exception(String.Format("Timed out while loading file {0} (Not an Assembly or in use?)", AssemblyPath));
                            }
                        }
                    }

                    try
                    {
                        m_Assembly = LoadFromStream(AssemblyStream);
                    } catch(Exception ex)
                    {
                        CurrentLoadingModPath = null;
                        throw new Exception(string.Format("Error occurred while loading assembly from stream. Path: {0} - Exception: {1}", AssemblyPath, ex));
                      
                    }

                    AssemblyStream.Close();

                    /*
                     * iterate through each type in Assembly to find which class implements the ISledgeMod interface
                     */
                    Type? InterfaceType = null;
                    foreach (Type ModType in m_Assembly.GetTypes())
                    {
                        if (ModType.GetInterface(nameof(ISledgeMod)) != null)
                        {
                            InterfaceType = ModType;
                            break;
                        }
                    }
                    if (InterfaceType == null)
                    {
                        Unload();
                        GC.Collect();
                        CurrentLoadingModPath = null;
                        throw new Exception("Mod does not implement ISledgeMod interface");
                    }

                    /*
                     * instantiate the class that implements ISledgeMod
                     */
                    ISledgeMod? InterfaceInstance = null;
                    try
                    {
                        InterfaceInstance = (ISledgeMod?)Activator.CreateInstance(InterfaceType);
                    } catch(Exception ex)
                    {
                        Unload();
                        GC.Collect();
                        CurrentLoadingModPath = null;
                        throw new Exception("Error while creating ISledgeMod interface: {0}", ex);
                    }

                    if (InterfaceInstance == null)
                    {
                        Unload();
                        GC.Collect();
                        CurrentLoadingModPath = null;
                        throw new Exception("Failed to instantiate interface class");
                    }
                    m_Interface = InterfaceInstance;

                    /*
                     * register the mod's callbacks
                     */
                    try
                    {
                        CallbackManager.RegisterCallbacks(this);
                    }
                    catch (Exception ex)
                    {
                        CallbackManager.UnregisterCallbacks(this);
                        Unload();
                        GC.Collect();
                        CurrentLoadingModPath = null;
                        Log.Error("Exception ocurred while registering callbacks: {0}", ex);
                        throw new Exception("Unable to register callbacks");
                    }

                    /*
                     * register the mod's lua functions
                     */
                    try
                    {
                        LuaFunctionManager.RegisterLuaFunctions(this);
                    }
                    catch (Exception ex)
                    {
                        CallbackManager.UnregisterCallbacks(this);
                        LuaFunctionManager.UnregisterLuaFunctions(this);
                        Unload();
                        GC.Collect();
                        CurrentLoadingModPath = null;
                        Log.Error("Exception ocurred while registering lua functions: {0}", ex);
                        throw new Exception("Unable to register lua functions");
                    }

                    /*
                     * add mod to list
                     */
                    ModList[m_AssemblyPath] = this;

                    /*
                     * invoke load function for mod
                     */
                    m_Interface.Load();

                    CurrentLoadingModPath = null;
                }
            }


            internal void UnloadMod()
            {
                lock (ModList)
                {
                    /*
                     * unregister callbacks, remove mod from list, invoke Unload function, unload assembly and collect GC
                     */
                    CallbackManager.UnregisterCallbacks(this);
                    LuaFunctionManager.UnregisterLuaFunctions(this);
                    ModList.Remove(m_AssemblyPath);
                    m_Interface.Unload();
                    Unload();
                    GC.Collect();
                }
            }

            ~ModContext() { Log.Verbose("ModContext GC called"); }
        }

        /*
         *  dependency resolver for mods
         */
        public static Assembly? ModDependencyResolver(AssemblyLoadContext LoadContext, AssemblyName DependencyName)
        {
            if (DependencyName.ToString() == typeof(ModManager).Assembly.GetName().ToString())
                return typeof(ModManager).Assembly;
            return null;
        }

        internal static ModContext? GetContextFromName(string Name)
        {
            lock (ModList)
            {
                foreach (ModContext Context in ModList.Values)
                    if (Context.Name == Name)
                        return Context;
            }
            return null;
        }

        internal static ModContext? GetContextFromAssembly(Assembly ModAssembly)
        {
            lock (ModList)
            {
                foreach (ModContext Context in ModList.Values)
                    if (Context.m_Assembly == ModAssembly)
                        return Context;
            }
            return null;
        }

        internal static string? GetModFolderFromAssembly(Assembly ModAssembly)
        {
            ModContext? Ctx = GetContextFromAssembly(ModAssembly);
            if (Ctx == null)
                return null;

            if (Ctx.m_DataFolder == null)
                return null;

            return Ctx.m_DataFolder;
        }

        internal static ModContext? GetContextFromPath(string Path)
        {
            lock (ModList)
            {
                foreach(ModContext Context in ModList.Values)
                    if (Context.m_AssemblyPath == Path)
                        return Context;
            }
            return null;
        }

        /*
         * watcher functions
         */
        internal static void OnFileCreated(object Sender, FileSystemEventArgs Args)
        {
            if (Path.GetExtension(Args.FullPath) != ".dll")
                return;
            try
            {
                new ModContext(Args.FullPath);
            }
            catch (Exception /*ex*/)
            {
                //Log.Error("Exception thrown while loading mod: {0} : {1}", Path.GetFileName(Args.FullPath), ex);
            }
            Log.General("Loaded mod: {0}", Path.GetFileName(Args.FullPath));
        }

        internal static void OnFileChanged(object Sender, FileSystemEventArgs Args)
        {
            ModContext? Ctx = GetContextFromPath(Args.FullPath);
            if (Ctx == null)
                return;

            if (Ctx.m_AssemblyLastWrite == File.GetLastWriteTime(Args.FullPath))
                return;

            Ctx.UnloadMod();

            try
            {
                new ModContext(Args.FullPath);
            }
            catch (Exception ex)
            {
                Log.Error("Exception thrown while reloading mod: {0} : {1}", Path.GetFileName(Args.FullPath), ex);
            }
            Log.General("Reloaded mod: {0}", Path.GetFileName(Args.FullPath));
        }

        internal static void OnFileDeleted(object Sender, FileSystemEventArgs Args)
        {
            ModContext? Ctx = GetContextFromPath(Args.FullPath);
            if (Ctx == null)
                return;

            Ctx.UnloadMod();
            Log.General("Unloaded mod: {0}", Path.GetFileName(Args.FullPath));
        }
    }
}