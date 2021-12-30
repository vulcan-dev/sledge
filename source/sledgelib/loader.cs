using SledgeLib;

internal class SledgeLoader
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

        Log.Verbose("Loading mods");
        if (!CModLoader.Init())
        {
            Log.Error("ModLoader failed to initialize");
            return false;
        }

        CModLoader.LoadMods();

        Log.General("Sledgelib succesfully initialized");
        return true;
    }
}
