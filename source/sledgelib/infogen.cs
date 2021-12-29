using System.Reflection;
using System.Text.Json;
using SledgeLib;

internal class CModInfoGenerator
{
    private static string[] ValidInitMethodNames = { "Init", "Start", "LoadMod", "StartMod", "ModInit" };

    internal static bool GenerateModInfo(Assembly ModAssembly, string sModInfoPath, string sModName)
    {
        Type[] ModTypes;
        try
        {
            ModTypes = ModAssembly.GetTypes();
        } catch (Exception ex)
        {
            Log.Error("Unable to get types for assembly {0}: {1}", sModName, ex.Message);
            return false;
        }

        string? sFinalTypeName = null;
        string? sFinalMethodName = null;

        foreach (Type ModType in ModTypes)
        {
            string? sTypeName = ModType.FullName;
            if (sTypeName == null)
                continue;

            if (sTypeName.StartsWith("Microsoft.") || sTypeName.StartsWith("System."))
                continue;

            MethodInfo[] Methods;
            try
            {
                Methods = ModType.GetMethods();
            }
            catch (Exception ex)
            {
                Log.Error("Error while getting methods for mod {0}: {1}", sTypeName, ex.Message);
                continue;
            }

            foreach (MethodInfo Method in Methods)
            {
                if (ValidInitMethodNames.Contains(Method.Name))
                {
                    Log.General("Found valid type and method name for {0}", sModName);
                    sFinalMethodName = Method.Name;
                    sFinalTypeName = sTypeName;
                    break;
                }
            }
        }

        if (string.IsNullOrEmpty(sFinalTypeName) || string.IsNullOrEmpty(sFinalMethodName))
        {
            return false;
        }

        CModLoader.SModInfo ModInfo = new CModLoader.SModInfo();
        ModInfo.sTypeName = sFinalTypeName; 
        ModInfo.sMethodName = sFinalMethodName;
        ModInfo.sModName = sModName;
        ModInfo.sModAuthor = "Unknown";

        string sSerializedModInfo = JsonSerializer.Serialize<CModLoader.SModInfo>(ModInfo);

        File.WriteAllText(sModInfoPath, sSerializedModInfo);
        Log.General("Generated mod info for {0}", sModName);

        return true;
    }
}