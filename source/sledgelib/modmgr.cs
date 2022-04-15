using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;

namespace SledgeLib
{
    internal class ModManager
    {
        internal static FileSystemWatcher? Watcher = null;
        internal static string? AssemblyLocation;
        internal static string? ModsFolder;

        internal static Dictionary<string, ModContext> ModList = new Dictionary<string, ModContext>();

        /*
         *  loads all available mods and sets up file watchers 
         */
        internal static void Init()
        {
            /*
             * get the required paths for loading mods
             */
            AssemblyLocation = Path.GetDirectoryName(typeof(ModManager).Assembly.Location);
            if (AssemblyLocation == null)
                throw new System.Exception("Unable to get assembly location");


            ModsFolder = AssemblyLocation + "\\..\\mods";
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

                ModList[ModCtx.m_Interface.GetName()] = ModCtx;

                Log.Verbose("Loaded mod: {0} by {1}", ModCtx.m_Interface.GetName(), ModCtx.m_Interface.GetAuthor());
            }

            /*
             * create watcher for handling mod loading / unloading / reloading
             */
            Watcher = new FileSystemWatcher(ModsFolder);

            Watcher.Created += OnFileCreated;
            Watcher.Changed += OnFileChanged;
            Watcher.Deleted += OnFileDeleted;

            Log.General("Loaded mods and registered watcher functions");
        }

        /*
         * class for the mod's information and context
         */
        internal class ModContext : AssemblyLoadContext
        {
            internal Assembly m_Assembly;
            internal DateTime m_AssemblyLastWrite;

            internal bool m_HasDataFolder = false;

            internal string m_DataFolder = "";
            internal string m_AssemblyName;
            internal string m_AssemblyPath;

            internal ISledgeMod m_Interface;

            public ModContext(string AssemblyPath)
            {
                m_AssemblyPath = AssemblyPath;
                m_AssemblyName = Path.GetFileNameWithoutExtension(AssemblyPath);
                m_AssemblyLastWrite = File.GetLastWriteTime(AssemblyPath);

                Resolving += ModDependencyResolver;
                
                /*
                 * load DLL from assembly so that the assembly may be deleted or changed after it's loaded
                 */
                FileStream AssemblyStream = File.OpenRead(AssemblyPath);
                m_Assembly = LoadFromStream(AssemblyStream);
                AssemblyStream.Close();

                /*
                 * iterate through each type in Assembly to find which class implements the ISledgeMod interface
                 */
                Type? InterfaceType = null;
                foreach (Type ModType in m_Assembly.GetTypes()) {
                    if (ModType.GetInterface(nameof(ISledgeMod)) != null)
                    {
                        InterfaceType = ModType;
                        break;
                    }
                }
                if (InterfaceType == null)
                    throw new Exception("Mod does not implement ISledgeMod interface");

                /*
                 * instantiate the class that implements ISledgeMod
                 */
                ISledgeMod? InterfaceInstance = (ISledgeMod?)Activator.CreateInstance(InterfaceType);
                if (InterfaceInstance == null)
                    throw new Exception("Failed to instantiate interface class");
                m_Interface = InterfaceInstance;

                /*
                 * check if mod specifies it's own folder (for data, dependencies, etc)
                 */
                if (Directory.Exists(ModsFolder + "\\" + m_AssemblyName))
                {
                    m_HasDataFolder = true;
                    m_DataFolder = ModsFolder + "\\" + m_AssemblyName;
                }
            }
        }

        /*
         *  dependency resolver for mods
         */
        private static Assembly? ModDependencyResolver(AssemblyLoadContext LoadContext, AssemblyName DependencyName)
        {
            if (DependencyName.ToString() == typeof(ModManager).Assembly.GetName().ToString())
                return typeof(ModManager).Assembly;

            return null;
        }

        /*
         * watcher functions
         */
        internal static void OnFileCreated(object Sender, FileSystemEventArgs Args)
        {
            Log.Verbose("file created: {0}", Args.FullPath);
        }

        internal static void OnFileChanged(object Sender, FileSystemEventArgs Args)
        {
            Log.Verbose("file changed: {0}", Args.FullPath);
        }

        internal static void OnFileDeleted(object Sender, FileSystemEventArgs Args)
        {
            Log.Verbose("file deleted: {0}", Args.FullPath);
        }
    }
}