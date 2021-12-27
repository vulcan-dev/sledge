using System.Collections.Concurrent;
using System.Runtime.InteropServices;

namespace SledgeLib
{
    public delegate void dCallback();

    internal class CCallbackManager
    {
        [DllImport("sledge.dll")] public static extern void RegisterCallback(ECallbackType CallbackType, dCallback CallbackFunc);

        internal static ConcurrentBag<CCallback> m_PlayerSpawnCallbacks = new ConcurrentBag<CCallback>();
        internal static ConcurrentBag<CCallback> m_PreUpdateCallbacks = new ConcurrentBag<CCallback>();
        internal static ConcurrentBag<CCallback> m_PostUpdateCallbacks = new ConcurrentBag<CCallback>();
        internal static ConcurrentBag<CCallback> m_PrePlayerUpdateCallbacks = new ConcurrentBag<CCallback>();
        internal static ConcurrentBag<CCallback> m_PostPlayerUpdateCallbacks = new ConcurrentBag<CCallback>();

        internal static void IterateCallbacks(ConcurrentBag<CCallback> Bag)
        {
            foreach (CCallback Callback in Bag)
            {
                if (!Callback.m_Active)
                    continue;

                try
                {
                    Callback.m_Callback();
                }
                catch (Exception e)
                {
                    Log.Error("Callback error: {0}", e.Message);
                    Callback.m_Active = false;
                }
            }
        }

        internal static void OnPlayerSpawn() { IterateCallbacks(m_PlayerSpawnCallbacks); }
        internal static dCallback PlayerSpawnCallback = new dCallback(OnPlayerSpawn);

        internal static void OnPreUpdate() { IterateCallbacks(m_PreUpdateCallbacks); }
        internal static dCallback PreUpdateCallback = new dCallback(OnPreUpdate);

        internal static void OnPostUpdate() { IterateCallbacks(m_PostUpdateCallbacks); }
        internal static dCallback PostUpdateCallback = new dCallback(OnPostUpdate);

        internal static void OnPrePlayerUpdate() { IterateCallbacks(m_PrePlayerUpdateCallbacks); }
        internal static dCallback PrePlayerUpdateCallback = new dCallback(OnPreUpdate);

        internal static void OnPostPlayerUpdate() { IterateCallbacks(m_PostPlayerUpdateCallbacks); }
        internal static dCallback PostPlayerUpdateCallback = new dCallback(OnPostPlayerUpdate);
    }

    public enum ECallbackType : uint
    {
        PlayerSpawn,
        PreUpdate,
        PostUpdate,
        PrePlayerUpdate,
        PostPlayerUpdate
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
}