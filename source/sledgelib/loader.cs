using System.Text.Json;
using System.Reflection;

internal class SledgeLoader
{
    public delegate IntPtr GetInternalAPIDelegate();
    public delegate bool InitDelegate(GetInternalAPIDelegate GetInternalAPI, string sModulePath);

    public static string? m_ModulePath;
    public static string? m_ModsPath;

    private struct SAssemblyConfig
    {
        public string sTypeName {  get; set; }
        public string sMethodName { get; set; }
    }

    /*
        to-do:
                (!) implement protection / restriction so modules can't do whatever they want
                 -  improve error checking, some of this feels way too overexagerated, but maybe not, who knows
    */
    private static bool LoadAssembly(string sAssemblyName)
    {
        SledgeLib.WriteLog("Loading and initializing assembly: " + sAssemblyName);

        string sConfig;
        try
        {
            sConfig = System.IO.File.ReadAllText(m_ModsPath + sAssemblyName + ".info.json");
        }
        catch (Exception e)
        {
            SledgeLib.WriteError("Failed to open info.json: " + e);
            return false;
        }

        SAssemblyConfig AssemblyConfig;
        try
        {
            AssemblyConfig = JsonSerializer.Deserialize<SAssemblyConfig>(sConfig);
        }
        catch (Exception e)
        {
            SledgeLib.WriteError("error while deserializing config: " + e.ToString());
            return false;
        }

        if (string.IsNullOrEmpty(AssemblyConfig.sTypeName) || string.IsNullOrEmpty(AssemblyConfig.sMethodName))
        {
            SledgeLib.WriteError("improperly formatted config.json (missing sTypeName and/or sMethodName)");
            return false;
        }

        SledgeLib.WriteLog("Loaded assembly config: (sTypeName: " + AssemblyConfig.sTypeName + " | sMethodName: " + AssemblyConfig.sMethodName + ")");

        Assembly vAssembly;
        try
        {
            vAssembly = Assembly.Load(sAssemblyName);
        }
        catch (Exception e)
        {
            SledgeLib.WriteError("LoadAssembly failed: " + e);
            return false;
        }
        SledgeLib.WriteLog("Loaded assembly: " + sAssemblyName);

        Type? AssemblyType;
        try
        {
            AssemblyType = vAssembly.GetType(AssemblyConfig.sTypeName, true, false);
        }
        catch (Exception e)
        {
            SledgeLib.WriteError("GetType failed: " + e);
            SledgeLib.WriteLog("Available types:");
            Type[] AssemblyTypes = vAssembly.GetTypes();
            foreach (Type type in AssemblyTypes)
            {
                SledgeLib.WriteLog("--> " + type.FullName);
            }

            return false;
        }

        if (AssemblyType == null)
        {
            SledgeLib.WriteError("AssemblyType was null");
            return false;
        }

        SledgeLib.WriteLog("Got assembly type: " + AssemblyType.ToString());

        MethodInfo? AssemblyMethod;
        try
        {
            AssemblyMethod = AssemblyType.GetMethod(AssemblyConfig.sMethodName);
        }
        catch (System.Exception e)
        {
            SledgeLib.WriteError("GetMethod failed: " + e);
            return false;
        }

        if (AssemblyMethod == null)
        {
            SledgeLib.WriteError("AssemblyMethod was null");
            return false;
        }

        SledgeLib.WriteLog("Got assembly method: " + AssemblyMethod.ToString());

        object? AssemblyInstance;
        try
        {
            AssemblyInstance = Activator.CreateInstance(AssemblyType);
        }
        catch (System.Exception e)
        {
            SledgeLib.WriteError("CreateInstance error:" + e);
            return false;
        }

        if (AssemblyInstance == null)
        {
            SledgeLib.WriteError("AssemblyInstance was null");
            return false;
        }

        SledgeLib.WriteLog("Got assembly instance: " + AssemblyInstance.ToString());

        try
        {
            AssemblyMethod.Invoke(AssemblyInstance, null);
        }
        catch (Exception e)
        {
            SledgeLib.WriteError("Method.Invoke error:" + e);
            return false;
        }

        SledgeLib.WriteLog("Fully loaded assembly: " + sAssemblyName);
        return true;
    }

    public static bool Init(GetInternalAPIDelegate GetInternalAPI, string sModulePath)
    {
        if (GetInternalAPI == null || sModulePath == null)
            return false;

        IntPtr pInternal = GetInternalAPI();
        if (!SledgeLib._SetInternal(pInternal))
            return false;

        SledgeLib.WriteLog("loading mods");

        m_ModulePath = sModulePath;
        m_ModsPath = sModulePath + "\\mods\\";

        LoadAssembly("sledge_examplemod");

        return true;
    }
}
