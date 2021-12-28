using System.Text.Json;
using System.Reflection;
using SledgeLib;

internal class CModLoader
{
    struct SModInfo
    {
        public string sTypeName { get; set; }
        public string sMethodName { get; set; }
    }

    internal static void LoadAssembly(string sModsPath, string sModName)
    {
        Log.General("loading assembly: {0}", sModName);

        string sConfig = "";
        try
        {
            sConfig = System.IO.File.ReadAllText(sModsPath + "\\" + sModName + ".info.json");
        } catch (Exception e)
        {
            Log.Error("Unable to load {0}.info.json: {1}", sModName, e.Message);
            return;
        }

        SModInfo ModInfo;
        try
        {
            ModInfo = JsonSerializer.Deserialize<SModInfo>(sConfig);
        } catch (Exception e)
        {
            Log.Error("Error while parsing {0}.info.json: {1}", sModName, e.Message);
            return;
        }

        if (string.IsNullOrEmpty(ModInfo.sTypeName) || string.IsNullOrEmpty(ModInfo.sMethodName))
        {
            Log.Error("Missing data for {0}.info.json (missing sTypeName and/or sMethodName)", sModName);
            return;
        }

        Assembly ModAssembly;
        try
        {
            ModAssembly = Assembly.Load(sModName);
        } catch (Exception e)
        {
            Log.Error("error while loading assembly: {0}.dll: {1}", sModName, e.Message);
            return;
        }

        Type? ModType = ModAssembly.GetType(ModInfo.sTypeName, false, false);
        
        if (ModType == null)
        {
            Log.Error("failed to get type: {0} for assembly: {1}.dll", ModInfo.sTypeName, sModName);
            return;
        }

        MethodInfo? ModMethod = ModType.GetMethod(ModInfo.sMethodName);
        if (ModMethod == null)
        {
            Log.Error("failed to get method: {0} for type: {1} in assembly: {2}.dll", ModInfo.sMethodName, ModInfo.sTypeName, sModName);
            return;
        }

        object? ModInstance = Activator.CreateInstance(ModType);
        if (ModInstance == null)
        {
            Log.Error("failed to create instance for type: {0} in assembly: {1}.dll", ModInfo.sTypeName, sModName);
            return;
        }

        try
        {
            ModMethod.Invoke(ModInstance, null);
        } catch (Exception e)
        {
            Log.Error("error while invoking method {0}@{1} - {2}: {3}", ModInfo.sMethodName, ModInfo.sTypeName, sModName, e.Message);
        }

        Log.General("successfully loaded mod: {0}", sModName);
        return;
    }

    internal static void LoadMods()
    {
        Assembly? ThisAssembly = Assembly.GetExecutingAssembly();

        if (ThisAssembly == null)
            return;

        string? sModsPath = Path.GetDirectoryName(ThisAssembly.Location);

        if (sModsPath == null)
            return;

        string[] ModFiles = Directory.GetFiles(sModsPath);
        foreach (string ModFile in ModFiles)
        {
            if (!ModFile.EndsWith(".dll"))
                continue;

            string sModName = Path.GetFileNameWithoutExtension(ModFile);
            if (sModName == "sledgelib")
                continue;

            LoadAssembly(sModsPath, sModName);
        }
    }
}