using SledgeLib;

internal class Loader
{
    /*
     * function that gets called from sledge_core once it's injected
     */
    public delegate void dGetInterface(ref UnmanagedInterface Interface);
    public static void GetInterface(ref UnmanagedInterface Interface)
    {
        Interface.Init = Loader.Init;
        Interface.Shutdown = Loader.Shutdown;

        Log.General("Sent unmanaged interface");
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