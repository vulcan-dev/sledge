using System.Collections.Concurrent;

public delegate void dCallback();

public enum ECallbackType : uint
{
    PlayerSpawn = 0,
    PreUpdate = 1,
    PostUpdate = 2,
    PrePlayerUpdate = 3,
    PostPlayerUpdate = 4
}

internal class CCallbackManager
{
    internal static ConcurrentBag<CCallback> m_PlayerSpawnCallbacks = new ConcurrentBag<CCallback>();
    internal static ConcurrentBag<CCallback> m_PreUpdateCallbacks = new ConcurrentBag<CCallback>();
    internal static ConcurrentBag<CCallback> m_PostUpdateCallbacks = new ConcurrentBag<CCallback>();
    internal static ConcurrentBag<CCallback> m_PrePlayerUpdateCallbacks = new ConcurrentBag<CCallback>();
    internal static ConcurrentBag<CCallback> m_PostPlayerUpdateCallbacks = new ConcurrentBag<CCallback>();

    internal static void OnPlayerSpawn()
    {
        foreach (CCallback Callback in m_PlayerSpawnCallbacks)
        {
            if (!Callback.m_Active)
                continue;

            try
            {
                Callback.m_Callback();
            } catch (Exception e)
            {
                SledgeLib.WriteError("Callback error: " + e.Message);
                Callback.m_Active = false;
            }
        }
    }
    internal static dCallback PlayerSpawnCallback = new dCallback(OnPlayerSpawn);

    internal static void OnPreUpdate()
    {
        foreach (CCallback Callback in m_PreUpdateCallbacks)
        {
            if (!Callback.m_Active)
                continue;

            try
            {
                Callback.m_Callback();
            }
            catch (Exception e)
            {
                SledgeLib.WriteError("Callback error: " + e.Message);
                Callback.m_Active = false;
            }
        }
    }
    internal static dCallback PreUpdateCallback = new dCallback(OnPreUpdate);


    internal static void OnPostUpdate()
    {
        foreach (CCallback Callback in m_PostUpdateCallbacks)
        {
            if (!Callback.m_Active)
                continue;

            try
            {
                Callback.m_Callback();
            }
            catch (Exception e)
            {
                SledgeLib.WriteError("Callback error: " + e.Message);
                Callback.m_Active = false;
            }
        }
    }
    internal static dCallback PostUpdateCallback = new dCallback(OnPostUpdate);

    internal static void OnPrePlayerUpdate()
    {
        foreach (CCallback Callback in m_PrePlayerUpdateCallbacks)
        {
            if (!Callback.m_Active)
                continue;

            try
            {
                Callback.m_Callback();
            }
            catch (Exception e)
            {
                SledgeLib.WriteError("Callback error: " + e.Message);
                Callback.m_Active = false;
            }
        }
    }
    internal static dCallback PrePlayerUpdateCallback = new dCallback(OnPreUpdate);


    internal static void OnPostPlayerUpdate()
    {
        foreach (CCallback Callback in m_PostPlayerUpdateCallbacks)
        {
            if (!Callback.m_Active)
                continue;

            try
            {
                Callback.m_Callback();
            }
            catch (Exception e)
            {
                SledgeLib.WriteError("Callback error: " + e.Message);
                Callback.m_Active = false;
            }
        }
    }
    internal static dCallback PostPlayerUpdateCallback = new dCallback(OnPostPlayerUpdate);
}

public class CCallback
{
    internal ECallbackType m_CallbackType;
    internal dCallback m_Callback;
    public bool m_Active;

    public CCallback(ECallbackType eType, dCallback Callback, bool bActive = true)
    {
        m_CallbackType = eType;
        m_Callback = Callback;
        m_Active = bActive;

        switch (eType)
        {
            case ECallbackType.PlayerSpawn:
                CCallbackManager.m_PlayerSpawnCallbacks.Add(this);
                break;
            case ECallbackType.PreUpdate:
                CCallbackManager.m_PreUpdateCallbacks.Add(this);
                break;
            case ECallbackType.PostUpdate:
                CCallbackManager.m_PostUpdateCallbacks.Add(this);
                break;
            case ECallbackType.PrePlayerUpdate:
                CCallbackManager.m_PrePlayerUpdateCallbacks.Add(this);
                break;
            case ECallbackType.PostPlayerUpdate:
                CCallbackManager.m_PostPlayerUpdateCallbacks.Add(this);
                break;
        }
    }
}