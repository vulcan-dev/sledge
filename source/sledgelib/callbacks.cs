using System.Runtime.InteropServices;

namespace SledgeLib
{
    public delegate void dCallback();

    internal class CCallbackManager
    {
        [DllImport("sledge.dll")] public static extern void RegisterCallback(ECallbackType CallbackType, dCallback CallbackFunc);

        internal static List<CCallback> m_PlayerSpawnCallbacks = new List<CCallback>();
        internal static List<CCallback> m_PreUpdateCallbacks = new List<CCallback>();
        internal static List<CCallback> m_PostUpdateCallbacks = new List<CCallback>();
        internal static List<CCallback> m_PrePlayerUpdateCallbacks = new List<CCallback>();
        internal static List<CCallback> m_PostPlayerUpdateCallbacks = new List<CCallback>();

        internal static void IterateCallbacks(List<CCallback> CallbackList)
        {
            foreach (CCallback Callback in CallbackList)
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
                    lock(CCallbackManager.m_PlayerSpawnCallbacks) { CCallbackManager.m_PlayerSpawnCallbacks.Add(this); }
                    break;
                case ECallbackType.PreUpdate:
                    lock (CCallbackManager.m_PreUpdateCallbacks) { CCallbackManager.m_PreUpdateCallbacks.Add(this); }
                    break;
                case ECallbackType.PostUpdate:
                    lock (CCallbackManager.m_PostUpdateCallbacks) { CCallbackManager.m_PostUpdateCallbacks.Add(this); }
                    break;
                case ECallbackType.PrePlayerUpdate:
                    lock (CCallbackManager.m_PrePlayerUpdateCallbacks) { CCallbackManager.m_PrePlayerUpdateCallbacks.Add(this); }
                    break;
                case ECallbackType.PostPlayerUpdate:
                    lock (CCallbackManager.m_PostPlayerUpdateCallbacks) { CCallbackManager.m_PostPlayerUpdateCallbacks.Add(this); }
                    break;
            }
        }

        ~CCallback()
        {
            switch (m_CallbackType)
            {
                case ECallbackType.PlayerSpawn:
                    lock (CCallbackManager.m_PlayerSpawnCallbacks) { CCallbackManager.m_PlayerSpawnCallbacks.Remove(this); }
                    break;
                case ECallbackType.PreUpdate:
                    lock (CCallbackManager.m_PreUpdateCallbacks) { CCallbackManager.m_PreUpdateCallbacks.Remove(this); }
                    break;
                case ECallbackType.PostUpdate:
                    lock (CCallbackManager.m_PostUpdateCallbacks) { CCallbackManager.m_PostUpdateCallbacks.Remove(this); }
                    break;
                case ECallbackType.PrePlayerUpdate:
                    lock (CCallbackManager.m_PrePlayerUpdateCallbacks) { CCallbackManager.m_PrePlayerUpdateCallbacks.Remove(this); }
                    break;
                case ECallbackType.PostPlayerUpdate:
                    lock (CCallbackManager.m_PostPlayerUpdateCallbacks) { CCallbackManager.m_PostPlayerUpdateCallbacks.Remove(this); }
                    break;
            }
        }
    }
}