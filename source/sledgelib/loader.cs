using SledgeLib;

internal class Loader
{
    public delegate UnmanagedInterface dGetInterface();
    internal static UnmanagedInterface Interface = new UnmanagedInterface();
    public static UnmanagedInterface GetInterface()
    {
        Interface.Init = Loader.Init;
        Interface.Shutdown = Loader.Shutdown;
        Interface.GetCallbackInterface = CallbackInvokers.GetCallbackInvokers;
        return Interface;
    }

    public static bool Init()
    {
        Log.General("Initializing mod manager");
        try { ModManager.Init(); } catch (System.Exception e) { Log.Error("Error while initializing the mod manager: {0}", e); }

        Log.General("Sledgelib successfully loaded");
        return true;
    }

    public static bool Shutdown()
    {
        return true;
    }
}