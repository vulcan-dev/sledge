using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace SledgeLib
{
    public enum ECallbackType
    {
        StateChange,
        LevelLoad,
        PreUpdate,
        PostUpdate,
        PrePlayerUpdate,
        PostPlayerUpdate
    }

    /*
     * attribute used for registering callbacks
     */
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = true)]
    public class Callback : Attribute
    {
        internal ECallbackType m_Type;
        public Callback(ECallbackType CBType)
        {
            m_Type = CBType;
        }
    }

    class CallbackManager
    {
        internal struct MethodRules
        {
            internal Type m_ReturnType;
            internal List<Type> m_Args;

            public MethodRules(Type RetType, params Type[]? Args)
            {
                m_ReturnType = RetType;
                m_Args = new List<Type>();
                if (Args != null)
                    m_Args.AddRange(Args);
            }
        }

        internal static Dictionary<ECallbackType, MethodRules> CallbackRules = new Dictionary<ECallbackType, MethodRules>()
        {
            { ECallbackType.StateChange, new MethodRules(typeof(void), typeof(EGameState))},
            { ECallbackType.LevelLoad, new MethodRules(typeof(void), typeof(string)) },
            { ECallbackType.PreUpdate, new MethodRules(typeof(void)) },
            { ECallbackType.PostUpdate, new MethodRules(typeof(void)) },
            { ECallbackType.PrePlayerUpdate, new MethodRules(typeof(void), typeof(float)) },
            { ECallbackType.PostPlayerUpdate, new MethodRules(typeof(void), typeof(float)) },
        };

        /*
         * verifies whether the method is valid or invalid for the callback
         */
        internal static bool VerifyCallbackMethod(MethodInfo CallbackMethod, ECallbackType CallbackType)
        {
            MethodRules Rules = CallbackRules[CallbackType];

            // check if the return type matches the callback's return type
            if (CallbackMethod.ReturnType != Rules.m_ReturnType)
                return false;

            // check if the parameters match the parameters required for the callback
            ParameterInfo[] MethodParams = CallbackMethod.GetParameters();

            List<Type> MethodTypes = new List<Type>();
            foreach (ParameterInfo Param in MethodParams)
                MethodTypes.Add(Param.ParameterType);

            return MethodTypes.SequenceEqual(Rules.m_Args);
        }

        /*
         * gets all the callbacks from a mod and registers them
         */
        internal static void RegisterCallbacks(ModManager.ModContext Ctx)
        {
            // iterate through the assembly's types, and find every method that uses the callback attribute
            foreach (Type ModType in Ctx.m_Assembly.GetTypes())
            {
                MethodInfo[] TypeMethods = ModType.GetMethods();
                foreach(MethodInfo ModMethodInfo in TypeMethods)
                {
                    Callback? CallbackAttr = ModMethodInfo.GetCustomAttribute<Callback>();
                    if (CallbackAttr == null)
                        continue;

                    if (!VerifyCallbackMethod(ModMethodInfo, CallbackAttr.m_Type))
                        throw new Exception(string.Format("Method {0} does not match the required method type for callback: {1}", ModMethodInfo.Name, CallbackAttr.m_Type));

                    AddCallbackToInvokerList(CallbackAttr.m_Type, ModMethodInfo);
                }
            }
        }

        /*
         * unregisters all the callbacks  belonging to a mod
         */
        internal static void UnregisterCallbacks(ModManager.ModContext Mod)
        {
            lock (CallbackInvokers.StateChangeCallbacks) { CallbackInvokers.StateChangeCallbacks.RemoveAll(CallbackMethod => (CallbackMethod.DeclaringType != null) && (CallbackMethod.DeclaringType.Assembly == Mod.m_Assembly)); }
            lock (CallbackInvokers.LevelLoadCallbacks) { CallbackInvokers.LevelLoadCallbacks.RemoveAll(CallbackMethod => (CallbackMethod.DeclaringType != null) && (CallbackMethod.DeclaringType.Assembly == Mod.m_Assembly)); }
            lock (CallbackInvokers.PreUpdateCallbacks) { CallbackInvokers.PreUpdateCallbacks.RemoveAll(CallbackMethod => (CallbackMethod.DeclaringType != null) && (CallbackMethod.DeclaringType.Assembly == Mod.m_Assembly)); }
            lock (CallbackInvokers.PostUpdateCallbacks) { CallbackInvokers.PostUpdateCallbacks.RemoveAll(CallbackMethod => (CallbackMethod.DeclaringType != null) && (CallbackMethod.DeclaringType.Assembly == Mod.m_Assembly)); }
            lock (CallbackInvokers.PrePlayerUpdateCallbacks) { CallbackInvokers.PrePlayerUpdateCallbacks.RemoveAll(CallbackMethod => (CallbackMethod.DeclaringType != null) && (CallbackMethod.DeclaringType.Assembly == Mod.m_Assembly)); }
            lock (CallbackInvokers.PostPlayerUpdateCallbacks) { CallbackInvokers.PostPlayerUpdateCallbacks.RemoveAll(CallbackMethod => (CallbackMethod.DeclaringType != null) && (CallbackMethod.DeclaringType.Assembly == Mod.m_Assembly)); }
        }

        /*
         * Adds the callback to any of the lists defined in callbackinvokers.cs
         */
        internal static void AddCallbackToInvokerList(ECallbackType CallbackType, MethodInfo CallbackMethod)
        {
            switch(CallbackType)
            {
                case ECallbackType.StateChange:
                    lock(CallbackInvokers.StateChangeCallbacks) { CallbackInvokers.StateChangeCallbacks.Add(CallbackMethod); }
                    break;
                case ECallbackType.LevelLoad:
                    lock (CallbackInvokers.LevelLoadCallbacks) { CallbackInvokers.LevelLoadCallbacks.Add(CallbackMethod); }
                    break;
                case ECallbackType.PreUpdate:
                    lock (CallbackInvokers.PreUpdateCallbacks) { CallbackInvokers.PreUpdateCallbacks.Add(CallbackMethod); }
                    break;
                case ECallbackType.PostUpdate:
                    lock (CallbackInvokers.PostUpdateCallbacks) { CallbackInvokers.PostUpdateCallbacks.Add(CallbackMethod); }
                    break;
                case ECallbackType.PrePlayerUpdate:
                    lock (CallbackInvokers.PrePlayerUpdateCallbacks) { CallbackInvokers.PrePlayerUpdateCallbacks.Add(CallbackMethod); }
                    break;
                case ECallbackType.PostPlayerUpdate:
                    lock (CallbackInvokers.PostPlayerUpdateCallbacks) { CallbackInvokers.PostPlayerUpdateCallbacks.Add(CallbackMethod); }
                    break;
            }
        }

    }
}