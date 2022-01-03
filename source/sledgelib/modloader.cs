using SledgeLib;
using System.Reflection;
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
    internal struct SRegisteredModInfo
    {
        public Assembly m_Assembly;
        public DateTime m_AssemblyLastWrite;
        public string m_Path;
        public string m_Name;
        public object m_Instance;
        public MethodInfo m_Load;
        public MethodInfo m_Unload;
        public MethodInfo? m_Reload;
        public AssemblyLoadContext m_LoadContext;
    }

    internal static List<SRegisteredModInfo> RegisteredMods = new List<SRegisteredModInfo>();
    internal static string? ModsPath = null;
    internal static FileSystemWatcher? Watcher = null;
    internal static Assembly ThisAssembly = typeof(CModLoader).Assembly;

    // Dependency resolver for loaded assemblies (Only takes into consideration sledgelib or files inside mods\moddir\dependencies\)
    private static Assembly? ModDependencyResolver(AssemblyLoadContext LoadContext, AssemblyName DependencyName)
    {
        if (DependencyName.ToString() == ThisAssembly.GetName().ToString())
            return ThisAssembly;
        SRegisteredModInfo ModInfo = RegisteredMods.Find(Info => Info.m_LoadContext.Name == LoadContext.Name);
        return ModInfo.m_LoadContext.LoadFromAssemblyPath(ModInfo.m_Path + "\\dependencies\\" + DependencyName.Name + ".dll");
    }

    private static SRegisteredModInfo LoadMod(string sModName, string sModPath)
    {
        SRegisteredModInfo RegModInfo = new SRegisteredModInfo();
        RegModInfo.m_Name = sModName;
        RegModInfo.m_Path = sModPath;
        RegModInfo.m_LoadContext = new AssemblyLoadContext(sModName, true);
        RegModInfo.m_LoadContext.Resolving += ModDependencyResolver;
        RegModInfo.m_AssemblyLastWrite = File.GetLastWriteTime(sModPath + "\\" + sModName + ".dll");

        FileStream AssemblyStream = File.OpenRead(sModPath + "\\" + sModName + ".dll");

        RegModInfo.m_Assembly = RegModInfo.m_LoadContext.LoadFromStream(AssemblyStream);
        AssemblyStream.Close();

        if (!File.Exists(sModPath + "\\" + sModName + ".info.json"))
        {
            Log.Warning("Mod {0} has no info, attempting to generate one", sModName);
            CInfoGen.Generate(RegModInfo.m_Assembly, sModPath, sModName);
        }

        string sConfig = File.ReadAllText(sModPath + "\\" + sModName + ".info.json");
        SModInfo ModInfo = JsonSerializer.Deserialize<SModInfo>(sConfig);

        Type? ModType = RegModInfo.m_Assembly.GetType(ModInfo.sTypeName, true);

        if (ModType == null)
            throw new Exception("Mod type not found");

        MethodInfo? LoadMethod = ModType.GetMethod(ModInfo.sLoadMethodName);
        if (LoadMethod == null)
            throw new Exception("Mod load method not found");

        MethodInfo? UnloadMethod = ModType.GetMethod(ModInfo.sUnloadMethodName);
        if (UnloadMethod == null)
            throw new Exception("Mod unload method not found");

        MethodInfo? ReloadMethod = null;
        if (!string.IsNullOrEmpty(ModInfo.sReloadMethodName))
            ReloadMethod = ModType.GetMethod(ModInfo.sReloadMethodName);

        object? ModInstance = Activator.CreateInstance(ModType);
        if (ModInstance == null)
            throw new Exception("Unable to instantiate mod type");

        RegModInfo.m_Instance = ModInstance;
        RegModInfo.m_Load = LoadMethod;
        RegModInfo.m_Unload = UnloadMethod;
        RegModInfo.m_Reload = ReloadMethod;

        lock (RegisteredMods) { RegisteredMods.Add(RegModInfo); }
        return RegModInfo;
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

            SRegisteredModInfo RegModInfo;
            try
            {
                RegModInfo = LoadMod(sModName, sModPath);
            }
            catch (Exception ex)
            {
                Log.Error("Error ocurred while registering mod {0}: {1}", sModName, ex.Message);
                continue;
            }

            try
            {
                RegModInfo.m_Load.Invoke(RegModInfo.m_Instance, null);
            } catch(Exception ex)
            {
                Log.Error("Error while invoking load method for mod {0}: {1}", RegModInfo.m_Name, ex.Message);
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

        LoadMod(sModName, sModPath);
    }

    internal static void OnModDeleted(object Sender, FileSystemEventArgs Args)
    {
        try
        {
            SRegisteredModInfo ModInfo = RegisteredMods.Find(Mod => (Mod.m_Path + "\\" + Mod.m_Name) + ".dll" == Args.FullPath.ToString());
            lock (RegisteredMods) { RegisteredMods.Remove(ModInfo); }
            try
            {
                ModInfo.m_Unload.Invoke(ModInfo.m_Instance, null);
            } catch (Exception ex)
            {
                Log.Error("Error while invoking unload method for mod {0}: {1}", ModInfo.m_Name, ex);
            }
        } catch { } // we don't care about the exception, as it could be any file that gets deleted
    }

    internal static void OnModChanged(object Sender, FileSystemEventArgs Args)
    {
        try
        {
            SRegisteredModInfo ModInfo = RegisteredMods.Find(Mod => (Mod.m_Path + "\\" + Mod.m_Name) + ".dll" == Args.FullPath.ToString());
            if (ModInfo.m_AssemblyLastWrite == File.GetLastWriteTime(Args.FullPath))
                return;

            try
            {
                ModInfo.m_Unload.Invoke(ModInfo.m_Instance, null);
            }
            catch (Exception ex)
            {
                Log.Error("Error while invoking unload method for mod {0}: {1}", ModInfo.m_Name, ex);
            }

            ModInfo.m_LoadContext.Unload();

            lock (RegisteredMods) { RegisteredMods.Remove(ModInfo); }

            ModInfo = LoadMod(ModInfo.m_Name, ModInfo.m_Path);

            MethodInfo RestartMethod;


            if (ModInfo.m_Reload != null)
                RestartMethod = ModInfo.m_Reload;
            else
                RestartMethod = ModInfo.m_Load;

            try
            {
                RestartMethod.Invoke(ModInfo.m_Instance, null);
            }
            catch (Exception ex)
            {
                Log.Error("Error while invoking load/reload method for mod {0}: {1}", ModInfo.m_Name, ex.Message);
            }

        }
        catch { } // we don't care about the exception, as it could be any file that gets updated
    }
}