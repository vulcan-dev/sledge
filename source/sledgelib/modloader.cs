using System.Reflection;
using System.Runtime.Loader;
using System.Text.Json;

using SledgeLib;

internal class CModLoader
{
    internal struct SModInfo
    {
        public string sModName { get; set; }
        public string sModAuthor { get; set; }
        public string sTypeName { get; set; }
        public string sMethodName { get; set; }
    }

    internal static AssemblyLoadContext ModLoadContext = new AssemblyLoadContext("ModLoadContext", false);
    internal static string? ModsPath = null;

    private static Assembly? _ModDependencyResolver(AssemblyLoadContext Context, AssemblyName Name)
    {
        Assembly ThisAssembly = typeof(CModLoader).Assembly;
        if (Name.ToString() == ThisAssembly.GetName().ToString())
            return ThisAssembly;

        Log.Error("Unable to load assembly dependency: {0}", Name.ToString());
        return null;
    }

    internal static bool Init()
    {
        Assembly ThisAssembly = typeof(CModLoader).Assembly;

        ModsPath = Path.GetDirectoryName(ThisAssembly.Location);
        if (ModsPath == null)
            return false;

        ModLoadContext.Resolving += _ModDependencyResolver;

        return true;
    }

    internal static void LoadMod(string sModPath, string sModName)
    {
        string sAssemblyPath = sModPath + "\\" + sModName + ".dll";
        Assembly ModAssembly;
        try
        {
            ModAssembly = CModLoader.ModLoadContext.LoadFromAssemblyPath(sAssemblyPath);
        }
        catch (Exception ex)
        {
            Log.Error("Error while loading assembly for mod {0}: {1}", sModName, ex.Message);
            return;
        }

        string sModInfoPath = sModPath + "\\" + sModName + ".info.json";
        if (!File.Exists(sModInfoPath))
        {
            Log.Warning("Mod {0} has no info, attempting to generate one", sModPath);
            if (!CModInfoGenerator.GenerateModInfo(ModAssembly, sModInfoPath, sModName))
            {
                Log.Error("Unable to auto generate config for {0}", sModName);
                return;
            }
        }

        string sConfig;
        try
        {
            sConfig = File.ReadAllText(sModInfoPath);
        } catch (Exception ex)
        {
            Log.Error("Error while reading config for mod {0}: {1}", sModName, ex.Message);
            return;
        }

        SModInfo ModInfo;
        try
        {
            ModInfo = JsonSerializer.Deserialize<SModInfo>(sConfig);
        } catch (Exception ex)
        {
            Log.Error("Error while parsing config for mod {0}: {1}", sModName, ex);
            return;
        }

        if (ModInfo.sTypeName == null || ModInfo.sMethodName == null)
            return;

        Type? ModType;
        try
        {
            ModType = ModAssembly.GetType(ModInfo.sTypeName, true);
        } catch (Exception ex)
        {
            Log.Error("Error while getting loader type for mod {0}: {1}", sModName, ex);
            return;
        }

        if (ModType == null)
        {
            Log.Error("Error while getting loader type for mod {0}: {1}", sModName, "GetType returned null");
            return;
        }

        MethodInfo? ModMethod = ModType.GetMethod(ModInfo.sMethodName);
        if (ModMethod == null)
        {
            Log.Error("Error while getting loader method for mod {0}: {1}", sModName, "GetMethod returned null");
            return;
        }

        object? ModInstance;
        try
        {
            ModInstance = Activator.CreateInstance(ModType);
        } catch (Exception ex)
        {
            Log.Error("Error while creating type instance for mod {0}: {1}", sModName, ex.Message);
            return;
        }
        
        if (ModInstance == null)
        {
            Log.Error("Error while creating type instance for mod {0}: {1}", sModName, "CreateInstance returned null");
            return;
        }

        try
        {
            ModMethod.Invoke(ModInstance, null);
        } catch (Exception ex)
        {
            Log.Error("Error while invoking loader method for mod {0}: {1}", ex.Message);
            return;
        }

        Log.General("Loaded mod: {0}", sModName);
    }

    internal static void LoadMods()
    {
        // ModsPath can't be 1, because Init already checks for it
        #pragma warning disable CS8604
        string[] ModFiles = Directory.GetFiles(ModsPath, "*.dll", SearchOption.AllDirectories);
        #pragma warning restore CS8604

        foreach (string sModFilePath in ModFiles)
        {
            string sModName = Path.GetFileNameWithoutExtension(sModFilePath);
            if (sModName == "sledgelib")
                continue;

            string? sModPath = Path.GetDirectoryName(sModFilePath);
            if (sModPath == null)
                continue;

            LoadMod(sModPath, sModName);
        }
    }
}