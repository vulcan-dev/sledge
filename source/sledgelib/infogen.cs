using System.Reflection;
using System.Text.Json;
using SledgeLib;

internal class CInfoGen
{
    private static string[] ValidLoadMethodNames = { "init", "start", "initmod", "loadmod", "startmod", "modinit", "modstart", "oninit", "oninitialize" };
    private static string[] ValidUnloadMethodNames = { "shutdown", "stop", "disable", "unload", "onunload", "onshutdown", "shutdownmod", "stopmod", "disablemod", "modshutdown", "modunload" };
    private static string[] ValidReloadMethodNames = { "reload", "restart", "reset", "onreload", "onrestart", "onreset", "restarting", "reloading", "resetting" };

    internal static void Generate(Assembly ModAssembly, string sModPath, string sModName)
    {
        CModLoader.SModInfo ModInfo = new CModLoader.SModInfo();
        ModInfo.sModName = sModName;
        ModInfo.sModAuthor = "Unknown";

        Type[] ModTypes = ModAssembly.GetTypes();

        foreach (Type ModType in ModTypes)
        {
            string? sTypeName = ModType.FullName;
            if (sTypeName == null)
                continue;

            if (sTypeName.StartsWith("Microsoft.") || sTypeName.StartsWith("System."))
                continue;

            MethodInfo[] Methods = ModType.GetMethods();

            ModInfo.sLoadMethodName = "";
            ModInfo.sUnloadMethodName = "";
            ModInfo.sReloadMethodName = "";
            ModInfo.sTypeName = sTypeName;

            foreach (MethodInfo Method in Methods)
            {
                if (ValidLoadMethodNames.Contains(Method.Name.ToLowerInvariant()))
                    ModInfo.sLoadMethodName = Method.Name;

                if (ValidUnloadMethodNames.Contains(Method.Name.ToLowerInvariant()))
                    ModInfo.sUnloadMethodName = Method.Name;

                if (ValidReloadMethodNames.Contains(Method.Name.ToLowerInvariant()))
                    ModInfo.sReloadMethodName = Method.Name;
            }

            if (!String.IsNullOrEmpty(ModInfo.sLoadMethodName) && !String.IsNullOrEmpty(ModInfo.sUnloadMethodName))
                break;
        }

        if (String.IsNullOrEmpty(ModInfo.sLoadMethodName) || String.IsNullOrEmpty(ModInfo.sUnloadMethodName))
            throw new Exception("Unable to find mod load/unload methods");

        string sSerializedModInfo = JsonSerializer.Serialize<CModLoader.SModInfo>(ModInfo);

        File.WriteAllText(sModPath + "\\" + sModName + ".info.json", sSerializedModInfo);
        Log.General("Generated mod info for mod: {0} - Load: {1} | Unload: {2} | Reload: {3} (Type: {4})", sModName, ModInfo.sUnloadMethodName, ModInfo.sLoadMethodName, ModInfo.sReloadMethodName, ModInfo.sTypeName);
    }
}