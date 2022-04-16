using System;
using System.Collections.Generic;
using System.Reflection;

namespace SledgeLib
{
    internal delegate void dStateChangeCallback(EGameState state);
    internal delegate void dStringCallback(string s);
    internal delegate void dCallback();

    internal class CallbackInvokers
    {
        internal static CallbackInvokersInterface Interface = new CallbackInvokersInterface();
        internal static CallbackInvokersInterface GetCallbackInvokers()
        {
            Interface.StateChange = OnStateChange;
            Interface.LevelLoad = OnLevelLoad;
            Interface.PreUpdate = OnPreUpdate;
            Interface.PostUpdate = OnPostUpdate;
            Interface.PrePlayerUpdate = OnPrePlayerUpdate;
            Interface.PostPlayerUpdate = OnPostPlayerUpdate;
            return Interface;
        }

        static void InvokeCallbacks(ref List<MethodInfo> CallbackList, object[]? Arguments)
        {
            foreach (MethodInfo CallbackMethod in CallbackList)
            {
                try
                {
                    CallbackMethod.Invoke(null, Arguments);
                }
                catch (Exception e)
                {
                    Log.Error("Error ocurred while invoking callback: {0}", e);
                    lock (CallbackList) { CallbackList.Remove(CallbackMethod); }
                }
            }
        }

        static internal List<MethodInfo> StateChangeCallbacks = new List<MethodInfo>();
        static void OnStateChange(EGameState eState)
        {
            InvokeCallbacks(ref StateChangeCallbacks, new object[] { eState } );
        }

        static internal List<MethodInfo> LevelLoadCallbacks = new List<MethodInfo>();
        static void OnLevelLoad(string sLevelName)
        {
            InvokeCallbacks(ref LevelLoadCallbacks, new object[] { sLevelName });
        }

        static internal List<MethodInfo> PreUpdateCallbacks = new List<MethodInfo>();
        static void OnPreUpdate()
        {
            InvokeCallbacks(ref PreUpdateCallbacks, null);
        }

        static internal List<MethodInfo> PostUpdateCallbacks = new List<MethodInfo>();
        static void OnPostUpdate()
        {
            InvokeCallbacks(ref PostUpdateCallbacks, null);
        }

        static internal List<MethodInfo> PrePlayerUpdateCallbacks = new List<MethodInfo>();
        static void OnPrePlayerUpdate()
        {
            InvokeCallbacks(ref PrePlayerUpdateCallbacks, null);
        }

        static internal List<MethodInfo> PostPlayerUpdateCallbacks = new List<MethodInfo>();
        static void OnPostPlayerUpdate()
        {
            InvokeCallbacks(ref PostPlayerUpdateCallbacks, null);
        }
    }
}