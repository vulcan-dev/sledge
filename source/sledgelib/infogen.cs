using System.Reflection;
using System.Text.Json;
using SledgeLib;

internal class CModInfoGenerator
{
    private static string[] ValidLoadMethodNames = { "init", "start", "initmod", "loadmod", "startmod", "modinit", "modstart" };
    private static string[] ValidUnloadMethodNames = { "shutdown", "stop", "disable", "unload", "shutdownmod", "stopmod", "disablemod", "modshutodwn", "modunload" };
    private static string[] ValidReloadMethodNames = { "reload", "restart", "reset", "onreload", "onrestart", "onreset", "restarting", "reloading", "resetting"};

    internal static bool GenerateModInfo(Assembly ModAssembly, string sModInfoPath, string sModName)
    {
        Type[] ModTypes;
        try
        {
            ModTypes = ModAssembly.GetTypes();
        } catch (Exception ex)
        {
            Log.Error("Error ocurred while getting assembly types for mod {0}: {1}", sModName, ex.Message);
            return false;
        }

        string? sFinalTypeName = null;
        string? sFinalLoadMethodName = null;
        string? sFinalUnloadMethodName = null;
        string? sFinalReloadMethodName = null;

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
                Log.Error("Error ocurred while getting methods for mod {0}: {1}", sTypeName, ex.Message);
                continue;
            }

            foreach (MethodInfo Method in Methods)
            {
                if (ValidLoadMethodNames.Contains(Method.Name.ToLowerInvariant()))
                {
                    Log.General("Found valid load method name for {0}", sModName);
                    sFinalLoadMethodName = Method.Name;
                    sFinalTypeName = sTypeName;
                }

                if (ValidUnloadMethodNames.Contains(Method.Name.ToLowerInvariant()))
                {
                    Log.General("Found valid unload method name for {0}", sModName);
                    sFinalUnloadMethodName = Method.Name;
                }

                if (ValidReloadMethodNames.Contains(Method.Name.ToLowerInvariant()))
                {
                    Log.General("Found valid reload method name for {0}", sModName);
                    sFinalReloadMethodName = Method.Name;
                }
            }
        }

        if (string.IsNullOrEmpty(sFinalTypeName) || string.IsNullOrEmpty(sFinalLoadMethodName) || string.IsNullOrEmpty(sFinalUnloadMethodName))
        {
            Log.Error("Error ocurred while generating config for mod {0}: {1}", sModName, "Unable to find Type, load method, and/or unload method");
            return false;
        }

        ModLoader.SModInfo ModInfo = new ModLoader.SModInfo();
        ModInfo.sTypeName = sFinalTypeName; 
        ModInfo.sLoadMethodName = sFinalLoadMethodName;
        ModInfo.sUnloadMethodName = sFinalUnloadMethodName;
        ModInfo.sModName = sModName;
        ModInfo.sModAuthor = "Unknown";
        ModInfo.sReloadMethodName = sFinalReloadMethodName != null ? sFinalReloadMethodName : "";

        string sSerializedModInfo = JsonSerializer.Serialize<ModLoader.SModInfo>(ModInfo);

        File.WriteAllText(sModInfoPath, sSerializedModInfo);
        Log.General("Generated mod info for {0}", sModName);

        return true;
    }
}