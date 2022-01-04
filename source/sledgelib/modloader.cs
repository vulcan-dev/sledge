using SledgeLib;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.Loader;
using System.Text.Json;

internal class CModLoader
{
    // struct that will be used for mod.info.json
    internal struct SModInfo
    {
        public string sModName { get; set; }
        public string sModAuthor { get; set; }
        public string sTypeName { get; set; }
        public string sLoadMethodName { get; set; }
        public string sUnloadMethodName { get; set; }
        public string sReloadMethodName { get; set; }
    }

    // struct that will be used internally for handling mods
    internal class CModContext : AssemblyLoadContext
    {
        public Assembly m_Assembly;
        public DateTime m_AssemblyLastWrite;
        public string m_Path;
        public string m_Name;
        public object? m_Instance;
        public Type? m_Type;
        public MethodInfo m_Load;
        public MethodInfo m_Unload;
        public MethodInfo? m_Reload;

        [MethodImpl(MethodImplOptions.NoInlining)]
        public CModContext(string sModName, string sModPath) : base(isCollectible: true)
        {
            m_Name = sModName;
            m_Path = sModPath;
            Resolving += ModDependencyResolver;
            m_AssemblyLastWrite = File.GetLastWriteTime(sModPath + "\\" + sModName + ".dll");

            FileStream AssemblyStream = File.OpenRead(sModPath + "\\" + sModName + ".dll");
            m_Assembly = LoadFromStream(AssemblyStream);
            AssemblyStream.Close();

            if (!File.Exists(sModPath + "\\" + sModName + ".info.json"))
            {
                Log.Warning("Mod {0} has no info, attempting to generate one", sModName);
                CInfoGen.Generate(m_Assembly, sModPath, sModName);
            }

            string sConfig = File.ReadAllText(sModPath + "\\" + sModName + ".info.json");
            SModInfo ModInfo = JsonSerializer.Deserialize<SModInfo>(sConfig);

            if (String.IsNullOrEmpty(ModInfo.sTypeName))
                throw new Exception("sTypeName was null");

            m_Type = m_Assembly.GetType(ModInfo.sTypeName, true);

            if (m_Type == null)
                throw new Exception("Mod type not found");

            MethodInfo? LoadMethod = m_Type.GetMethod(ModInfo.sLoadMethodName);
            if (LoadMethod == null)
                throw new Exception("Mod load method not found");

            MethodInfo? UnloadMethod = m_Type.GetMethod(ModInfo.sUnloadMethodName);
            if (UnloadMethod == null)
                throw new Exception("Mod unload method not found");

            MethodInfo? ReloadMethod = null;
            if (!string.IsNullOrEmpty(ModInfo.sReloadMethodName))
                ReloadMethod = m_Type.GetMethod(ModInfo.sReloadMethodName);

            object? ModInstance = Activator.CreateInstance(m_Type);
            if (ModInstance == null)
                throw new Exception("Unable to instantiate mod type");

            m_Instance = ModInstance;
            m_Load = LoadMethod;
            m_Unload = UnloadMethod;
            m_Reload = ReloadMethod;

            lock (RegisteredMods) { RegisteredMods.Add(this); }
        }

        ~CModContext()
        {
            Log.General("Context destructor got called");
        }
    }

    internal static List<CModContext> RegisteredMods = new List<CModContext>();
    internal static string? ModsPath = null;
    internal static FileSystemWatcher? Watcher = null;
    internal static Assembly ThisAssembly = typeof(CModLoader).Assembly;

    // Dependency resolver for loaded assemblies (Only takes into consideration sledgelib or files inside mods\moddir\dependencies\)
    private static Assembly? ModDependencyResolver(AssemblyLoadContext LoadContext, AssemblyName DependencyName)
    {
        if (DependencyName.ToString() == ThisAssembly.GetName().ToString())
            return ThisAssembly;

        CModContext? ModCtx = RegisteredMods.Find(Ctx => { return Ctx.Name == LoadContext.Name; });
        
        if (ModCtx == null)
            return null;

        return ModCtx.LoadFromAssemblyPath(ModCtx.m_Path + "\\dependencies\\" + DependencyName.Name + ".dll");
    }

    internal static void Init()
    {
        string? sAssemblyLocation = Path.GetDirectoryName(ThisAssembly.Location);
        if (sAssemblyLocation == null)
            throw new Exception("Unable to get assembly location");

        // Get all mod files and load them
        EnumerationOptions EnumOptions = new EnumerationOptions();
        EnumOptions.RecurseSubdirectories = true;
        EnumOptions.MaxRecursionDepth = 1;
        string[] ModFiles = Directory.GetFiles(sAssemblyLocation, "*.dll", EnumOptions);

        foreach (string sModFilePath in ModFiles)
        {
            string sModName = Path.GetFileNameWithoutExtension(sModFilePath);

            if (sModName == "sledgelib")
                continue;

            string? sModPath = Path.GetDirectoryName(sModFilePath);
            if (sModPath == null)
                continue;

            if (Path.GetFileName(sModPath) == "dependencies")
                continue;

            CModContext ModCtx;
            try
            {
                ModCtx = new CModContext(sModName, sModPath);
            }
            catch (Exception ex)
            {
                Log.Error("Error ocurred while registering mod {0}: {1}", sModName, ex);
                continue;
            }

            try
            {
                ModCtx.m_Load.Invoke(ModCtx.m_Instance, null);
            } catch(Exception ex)
            {
                Log.Error("Error while invoking load method for mod {0}: {1}", ModCtx.m_Name, ex);
                continue;
            }

            Log.General("Loaded mod: {0}", sModName);
        }

        // Create file watcher for hot-reloading / unloading
        Watcher = new FileSystemWatcher(sAssemblyLocation);
        Watcher.IncludeSubdirectories = true;
        Watcher.EnableRaisingEvents = true;

        Watcher.NotifyFilter = NotifyFilters.Attributes | NotifyFilters.DirectoryName | NotifyFilters.FileName | NotifyFilters.LastAccess | NotifyFilters.LastWrite;

        Watcher.Deleted += OnModDeleted;
        Watcher.Created += OnModCreated;
        Watcher.Changed += OnModChanged;
    }

    /*
     *  watcher functions
     */
    internal static void OnModCreated(object Sender, FileSystemEventArgs Args)
    {
        string? sModPath = Path.GetDirectoryName(Args.FullPath);
        string sModName = Path.GetFileNameWithoutExtension(Args.FullPath);

        if (sModPath == null)
            return;

        if (Path.GetFileName(sModPath) == "dependencies")
            return;

        new CModContext(sModName, sModPath);
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    internal static void OnModDeleted(object Sender, FileSystemEventArgs Args)
    {
        try
        {
            CModContext? ModCtx = RegisteredMods.Find(Mod => (Mod.m_Path + "\\" + Mod.m_Name) + ".dll" == Args.FullPath.ToString());
            if (ModCtx == null)
                return;

            lock (RegisteredMods) { RegisteredMods.Remove(ModCtx); }
            try
            {
                ModCtx.m_Unload.Invoke(ModCtx.m_Instance, null);
            } catch (Exception ex)
            {
                Log.Error("Error while invoking unload method for mod {0}: {1}", ModCtx.m_Name, ex);
            }

            ModCtx.Unload();
            GC.Collect();
        }
        catch { } // we don't care about the exception, as it could be any file that gets deleted
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    internal static void OnModChanged(object Sender, FileSystemEventArgs Args)
    {
        try
        {
            CModContext? ModCtx = RegisteredMods.Find(Ctx => { return Ctx.m_Path + "\\" + Ctx.m_Name + ".dll" == Args.FullPath.ToString(); });

            if (ModCtx == null)
                return;

            if (string.IsNullOrEmpty(ModCtx.m_Name))
                return;

            if (ModCtx.m_AssemblyLastWrite == File.GetLastWriteTime(Args.FullPath))
                return;

            WeakReference OldModCtx = new WeakReference(ModCtx);

            lock (RegisteredMods) { RegisteredMods.Remove(ModCtx); }

            if (ModCtx.m_Instance == null)
            {
                ModCtx.Unload();
                GC.Collect();
                Log.Error("Mod has no load context");
            } else
            {
                try
                {
                    ModCtx.m_Unload.Invoke(ModCtx.m_Instance, null);
                }
                catch (Exception ex)
                {
                    Log.Error("Error while invoking unload method for mod {0}: {1}", ModCtx.m_Name, ex);
                    ModCtx.Unload();
                    ModCtx = null;

                    for (int i=0; OldModCtx.IsAlive && (i < 10); i++)
                    {
                        GC.Collect();
                        GC.WaitForPendingFinalizers();
                    }

                    return;
                }
            }
            ModCtx.Unload();
            ModCtx = null;

            for (int i = 0; OldModCtx.IsAlive && (i < 10); i++)
            {
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }

            string? sModPath = Path.GetDirectoryName(Args.FullPath);
            string sModName = Path.GetFileNameWithoutExtension(Args.FullPath);

            if (sModPath == null)
                return;

            ModCtx = new CModContext(sModName, sModPath);
            MethodInfo RestartMethod;

            if (ModCtx.m_Reload != null)
                RestartMethod = ModCtx.m_Reload;
            else
                RestartMethod = ModCtx.m_Load;

            try
            {
                RestartMethod.Invoke(ModCtx.m_Instance, null);
            }
            catch (Exception ex)
            {
                Log.Error("Error while invoking load/reload method for mod {0}: {1}", ModCtx.m_Name, ex);
            }

        }
        catch { } // we don't care about the exception, as it could be any file that gets updated
    }
}