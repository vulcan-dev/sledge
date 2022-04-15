using SledgeLib;

internal class Loader
{
    /*
     * function that gets called from sledge_core once it's injected
     */
    public delegate bool dInit();
    public static bool Init()
    {
        Log.General("Initializing mod manager");
        try { ModManager.Init(); } catch (System.Exception e) { Log.Error("Error while initializing the mod manager: {0}", e); }

        Log.General("Sledgelib successfully loaded");
        return true;
    }
}