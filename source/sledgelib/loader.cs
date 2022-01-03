using SledgeLib;

internal class CSledgeLoader
{
    public delegate bool dInit();

    public static bool Init()
    {
        Log.Verbose("Initializing binds");
        CBindManager.RegisterInputReader(CBindManager.InputReader);

        Log.Verbose("Initializing callbacks");
        CCallbackManager.RegisterCallback(ECallbackType.PlayerSpawn, CCallbackManager.PlayerSpawnCallback);
        CCallbackManager.RegisterCallback(ECallbackType.PreUpdate, CCallbackManager.PreUpdateCallback);
        CCallbackManager.RegisterCallback(ECallbackType.PostUpdate, CCallbackManager.PostUpdateCallback);
        CCallbackManager.RegisterCallback(ECallbackType.PrePlayerUpdate, CCallbackManager.PrePlayerUpdateCallback);
        CCallbackManager.RegisterCallback(ECallbackType.PostPlayerUpdate, CCallbackManager.PostPlayerUpdateCallback);
        CCallbackManager.RegisterCallback(ECallbackType.StateChange, CCallbackManager.StateChangeCallback);
        CCallbackManager.RegisterCallback(ECallbackType.Tick, CCallbackManager.TickCallback);

        Log.Verbose("Registering internal callbacks");
        InternalCallbacks.cb_StateChange = new CCallback(ECallbackType.StateChange, InternalCallbacks.cb_StateChangeFunc);

        Log.Verbose("Loading mods");
        try
        {
            CModLoader.Init();
        }
        catch (Exception e)
        {
            Log.General("CModLoader.Init() failed: {0}", e.Message);
            return false;
        }

        Log.General("Sledgelib succesfully initialized");
        return true;
    }
}
