using SledgeLib;

internal class SledgeLoader
{
    public delegate bool dInit();

    public static bool Init()
    {
        Log.General("initializing binds");
        CBindManager.RegisterInputReader(CBindManager.InputReader);

        Log.General("initializing callbacks");
        CCallbackManager.RegisterCallback(ECallbackType.PlayerSpawn, CCallbackManager.PlayerSpawnCallback);
        CCallbackManager.RegisterCallback(ECallbackType.PreUpdate, CCallbackManager.PreUpdateCallback);
        CCallbackManager.RegisterCallback(ECallbackType.PostUpdate, CCallbackManager.PostUpdateCallback);
        CCallbackManager.RegisterCallback(ECallbackType.PrePlayerUpdate, CCallbackManager.PrePlayerUpdateCallback);
        CCallbackManager.RegisterCallback(ECallbackType.PostPlayerUpdate, CCallbackManager.PostPlayerUpdateCallback);

        Log.General("loading mods");
        if (!CModLoader.Init())
        {
            Log.Error("ModLoader failed to initialize");
            return false;
        }

        CModLoader.LoadMods();

        Log.General("sledgelib succesfully initialized");
        return true;
    }
}