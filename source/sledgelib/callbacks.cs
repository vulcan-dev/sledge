using System.Runtime.InteropServices;

namespace SledgeLib
{
    public delegate void dCallback();
    public delegate void dUIntCallback(uint iValue);

    public enum ECallbackType : uint
    {
        PlayerSpawn,
        PreUpdate,
        PostUpdate,
        PrePlayerUpdate,
        PostPlayerUpdate,
        StateChange
    }

    public class CCallback
    {
        internal ECallbackType m_Type;
        internal dynamic m_Callback;
        public bool m_Active;

        public CCallback(ECallbackType eType, dCallback Callback, bool bActive = true)
        {
            m_Type = eType;
            m_Callback = Callback;
            m_Active = bActive;

            switch (m_Type)
            {
                case ECallbackType.PlayerSpawn:
                    lock (CCallbackManager.m_PlayerSpawnCallbacks) { CCallbackManager.m_PlayerSpawnCallbacks.Add(this); }
                    break;
                case ECallbackType.PreUpdate:
                    lock (CCallbackManager.m_PreUpdateCallbacks) { CCallbackManager.m_PreUpdateCallbacks.Add(this); }
                    return;
                case ECallbackType.PostUpdate:
                    lock (CCallbackManager.m_PostUpdateCallbacks) { CCallbackManager.m_PostUpdateCallbacks.Add(this); }
                    return;
                case ECallbackType.PrePlayerUpdate:
                    lock (CCallbackManager.m_PrePlayerUpdateCallbacks) { CCallbackManager.m_PrePlayerUpdateCallbacks.Add(this); }
                    return;
                case ECallbackType.PostPlayerUpdate:
                    lock (CCallbackManager.m_PostPlayerUpdateCallbacks) { CCallbackManager.m_PostPlayerUpdateCallbacks.Add(this); }
                    return;

            }

            Log.Error("Delegate does not match callback type");
        }

        public CCallback(ECallbackType eType, dUIntCallback Callback, bool bActive = true)
        {
            m_Type = eType;
            m_Callback = Callback;
            m_Active = bActive;

            switch (m_Type)
            {
                case ECallbackType.StateChange:
                    lock (CCallbackManager.m_StateChangeCallbacks) { CCallbackManager.m_StateChangeCallbacks.Add(this); }
                    return;
            }

            Log.Error("Delegate does not match callback type");
        }


        public void Unregister()
        {
            switch (m_Type)
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
                case ECallbackType.StateChange:
                    lock (CCallbackManager.m_StateChangeCallbacks) { CCallbackManager.m_StateChangeCallbacks.Remove(this); }
                    break;
            }
        }
    }

    internal class CCallbackManager
    {
        [DllImport("sledge.dll")] public static extern void RegisterCallback(ECallbackType CallbackType, dCallback CallbackFunc);
        [DllImport("sledge.dll")] public static extern void RegisterCallback(ECallbackType CallbackType, dUIntCallback CallbackFunc);

        internal static List<CCallback> m_PlayerSpawnCallbacks = new List<CCallback>();
        internal static List<CCallback> m_PreUpdateCallbacks = new List<CCallback>();
        internal static List<CCallback> m_PostUpdateCallbacks = new List<CCallback>();
        internal static List<CCallback> m_PrePlayerUpdateCallbacks = new List<CCallback>();
        internal static List<CCallback> m_PostPlayerUpdateCallbacks = new List<CCallback>();
        internal static List<CCallback> m_StateChangeCallbacks = new List<CCallback>();

        internal static void IterateCallbacks(List<CCallback> CallbackList)
        {
            foreach (dynamic Callback in CallbackList)
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

        internal static void IterateCallbacksOneArg(List<CCallback> CallbackList, dynamic Arg)
        {
            foreach (dynamic Callback in CallbackList)
            {
                if (!Callback.m_Active)
                    continue;

                try
                {
                    Callback.m_Callback(Arg);
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
        internal static dCallback PrePlayerUpdateCallback = new dCallback(OnPrePlayerUpdate);
        internal static void OnPostPlayerUpdate() { IterateCallbacks(m_PostPlayerUpdateCallbacks); }
        internal static dCallback PostPlayerUpdateCallback = new dCallback(OnPostPlayerUpdate);

        internal static void OnStateChange(uint iState) { IterateCallbacksOneArg(m_StateChangeCallbacks, iState); }
        internal static dUIntCallback StateChangeCallback = new dUIntCallback(OnStateChange);

    }
}