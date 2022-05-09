using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;

/*
 * TO-DO: iterate every state and register function from there, also remove from vFunctions @ luahelpers.cpp
 */
namespace SledgeLib
{
    /*
     * attribute used for registering lua functions
     */
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = true)]
    public class LuaFunction : Attribute
    {
        internal string m_FunctionName;

        public LuaFunction(string FunctionName)
        {
            m_FunctionName = FunctionName;
        }
    }

    public class Lua
    {
        public enum VarType
        {
            LUA_TNIL,
            LUA_TBOOLEAN,
            LUA_TLIGHTUSERDATA,
            LUA_TNUMBER,
            LUA_TSTRING,
            LUA_TTABLE,
            LUA_TFUNCTION,
            LUA_TUSERDATA,
            LUA_TTHREAD
        }

        [DllImport("sledge_core.dll")]
        internal static extern int _lua_tointeger(IntPtr pL, int Index);
        [DllImport("sledge_core.dll")]
        internal static extern bool _lua_toboolean(IntPtr pL, int Index);
        [DllImport("sledge_core.dll")]
        internal static extern double _lua_tonumber(IntPtr pL, int Index);
        [DllImport("sledge_core.dll")]
        internal static extern StringBuilder _lua_tolstring(IntPtr pL, int Index);

        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pushnil(IntPtr pL);
        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pushnumber(IntPtr pL, double Value);
        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pushinteger(IntPtr pL, int Value);
        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pushboolean(IntPtr pL, bool Value);
        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pushlstring(IntPtr pL, string Value);

        [DllImport("sledge_core.dll")]
        internal static extern void _lua_getfield(IntPtr pL, int Index, string Key);
        [DllImport("sledge_core.dll")]
        internal static extern int _lua_gettop(IntPtr pL);

        [DllImport("sledge_core.dll")]
        internal static extern int _lua_pcall(IntPtr pL, int Args, int Results, int Err);
        [DllImport("sledge_core.dll")]
        internal static extern void _lua_call(IntPtr pL, int Args, int Results);

        [DllImport("sledge_core.dll")]
        internal static extern Lua.VarType _lua_type(IntPtr pL, int Index);

        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pop(IntPtr pL, int Elements);

        [DllImport("sledge_core.dll")]
        internal static extern IntPtr _GetLuaStateFromSC(IntPtr pSC);

        [DllImport("sledge_core.dll")]
        internal static extern uint _GetScriptCount();
        [DllImport("sledge_core.dll")]
        internal static extern void _WriteSCArray([MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] IntPtr[] pScriptArray);

        internal static int GLOBALSINDEX = -10002;

        internal static List<Type> AllowedLuaTypes = new List<Type>()
        {
            typeof(int), typeof(string), typeof(bool), typeof(float), typeof(double)
        };

        internal struct InvokeInfo
        {
            internal string m_FunctionName;
            internal object[] m_Arguments;
        }

        public static string GetCaller()
        {
            if (LuaFunctionManager.CurrentSC == null)
                return "";

            StringBuilder CallerSB = LuaFunctionManager._GetSCPath((IntPtr)LuaFunctionManager.CurrentSC);

            return CallerSB.ToString();
        }

        internal static List<InvokeInfo> InvokeQueue = new List<InvokeInfo>();
        internal static void UpdateInvokeQueue()
        {
            lock (InvokeQueue)
            {
                if (InvokeQueue.Count == 0)
                    return;

                IntPtr[] ScriptArray = new IntPtr[_GetScriptCount()];
                _WriteSCArray(ScriptArray);

                foreach (InvokeInfo Info in InvokeQueue)
                {
                    foreach (IntPtr pSC in ScriptArray)
                    {
                        IntPtr pL = _GetLuaStateFromSC(pSC);

                        _lua_getfield(pL, GLOBALSINDEX, Info.m_FunctionName);

                        if (_lua_type(pL, -1) != VarType.LUA_TFUNCTION)
                        {
                            _lua_pop(pL, 1);
                            continue;
                        }

                        foreach (object Arg in Info.m_Arguments)
                        {
                            switch (Arg.GetType().Name)
                            {
                                case "String":
                                    Lua._lua_pushlstring(pL, (string)Arg);
                                    break;
                                case "Int32":
                                    Lua._lua_pushinteger(pL, (int)Arg);
                                    break;
                                case "Boolean":
                                    Lua._lua_pushboolean(pL, (bool)Arg);
                                    break;
                                case "Single":
                                    Lua._lua_pushnumber(pL, Convert.ToDouble(Arg));
                                    break;
                                case "Double":
                                    Lua._lua_pushnumber(pL, (double)Arg);
                                    break;
                                default:
                                    Log.Error("Unknown invoke type: {0}", Arg.GetType().Name);
                                    break;
                            }
                        }

                        _lua_pcall(pL, _lua_gettop(pL) - 1, 0, 0);
                    }
                }

                InvokeQueue.Clear();
            }
        }

        internal static void Init()
        {
            MethodInfo? InvokeQueueMethod = typeof(Lua).GetMethod("UpdateInvokeQueue", BindingFlags.NonPublic | BindingFlags.Static );
            if (InvokeQueueMethod != null)
                CallbackManager.AddCallbackToInvokerList(ECallbackType.PreUpdate, InvokeQueueMethod);
            else
                throw new Exception("Unable to get UpdateInvokeQueue method info");
        }

        public static void Invoke(string FunctionName, params object[] Args)
        {
            InvokeInfo Info;
            Info.m_FunctionName = FunctionName;
            Info.m_Arguments = Args;

            foreach(object Arg in Args)
            {
                if (!AllowedLuaTypes.Contains(Arg.GetType()))
                    throw new Exception(String.Format("Invalid arg type passed to Lua.Invoke: {0}", Arg.GetType()));
            }

            lock(InvokeQueue)
                InvokeQueue.Add(Info);
        }
    }

    internal static class LuaFunctionManager
    {

        [DllImport("sledge_core.dll")]
        internal static extern void _RegisterLuaFunctionInternal(string FunctionName);
        [DllImport("sledge_core.dll")]
        internal static extern void _UnregisterLuaFunctionInternal(string FunctionName);

        [DllImport("sledge_core.dll")]
        internal static extern StringBuilder _GetSCPath(IntPtr pScriptCore);

        internal static IntPtr? CurrentSC = null;

        internal struct RegisteredLuaFunction
        {
            public string m_FunctionName;
            public MethodInfo m_Method;
            public ModManager.ModContext m_Context;
            public Type m_ReturnType;
            public List<Type> m_Args;

            public RegisteredLuaFunction(MethodInfo Method, ModManager.ModContext Ctx, LuaFunction Attr)
            {
                m_Args = new List<Type>();
                m_ReturnType = Method.ReturnType;
                m_FunctionName = Attr.m_FunctionName;
                m_Method = Method;
                m_Context = Ctx;
            }
        }

        internal static Dictionary<string, RegisteredLuaFunction> LuaFunctions = new Dictionary<string, RegisteredLuaFunction>();

        /*
         * function wrapper in charge of parsing lua arguments, invoking the C# method and returns
         */
        internal static int FunctionWrapper(IntPtr pSC, IntPtr pL, string FunctionName)
        {
            if (!LuaFunctions.ContainsKey(FunctionName))
                return 0;

            CurrentSC = pSC;

            RegisteredLuaFunction LuaFunc = LuaFunctions[FunctionName];

            object[] ArgsArray = { };

            if (LuaFunc.m_Args.Count > 0)
            {
                List<object> Args = new List<object>();
                int Index = 1;
                foreach (Type ArgumentType in LuaFunc.m_Args)
                {
                    switch (ArgumentType.Name) // hacky, maybe change this
                    {
                        case "String":
                            Args.Add(Lua._lua_tolstring(pL, Index).ToString());
                            break;
                        case "Int32":
                            Args.Add(Lua._lua_tointeger(pL, Index));
                            break;
                        case "Boolean":
                            Args.Add(Lua._lua_toboolean(pL, Index));
                            break;
                        case "Single":
                            Args.Add((float)Lua._lua_tonumber(pL, Index));
                            break;
                        case "Double":
                            Args.Add(Lua._lua_tonumber(pL, Index));
                            break;
                        default:
                            Log.Error("Unknown arg type: {0}", ArgumentType.Name);
                            return 0;
                    }
                    Index++;
                }

                ArgsArray = Args.ToArray();
            }

            try
            {
                object? Return = LuaFunc.m_Method.Invoke(null, ArgsArray);
                if (Return == null)
                    return 0;

                switch (LuaFunc.m_ReturnType.Name)
                {
                    case "String":
                        Lua._lua_pushlstring(pL, (string)Return);
                        break;
                    case "Int32":
                        Lua._lua_pushinteger(pL, (int)Return);
                        break;
                    case "Boolean":
                        Lua._lua_pushboolean(pL, (bool)Return);
                        break;
                    case "Single":
                        Lua._lua_pushnumber(pL, Convert.ToDouble(Return));
                        break;
                    case "Double":
                        Lua._lua_pushnumber(pL, (double)Return);
                        break;
                    default:
                        Log.Error("Unknown return type: {0}", LuaFunc.m_ReturnType.Name);
                        return 0;
                }

                return 1;
            }
            catch (Exception e)
            {
                Log.Error("Error on lua function {0}: {1}", LuaFunc.m_FunctionName, e);
                return 0;
            }
        }

        /*
         * gets all methods with the LuaFunction attribute and adds them to a list
         */
        internal static void RegisterLuaFunctions(ModManager.ModContext Ctx)
        {
            // iterate through the assembly's types, and find every method that is a lua function
            foreach (Type ModType in Ctx.m_Assembly.GetTypes())
            {
                MethodInfo[] TypeMethods = ModType.GetMethods();
                foreach (MethodInfo ModMethodInfo in TypeMethods)
                {
                    LuaFunction? LuaAttr = ModMethodInfo.GetCustomAttribute<LuaFunction>();
                    if (LuaAttr == null)
                        continue;

                    RegisteredLuaFunction LuaFunc = new RegisteredLuaFunction(ModMethodInfo, Ctx, LuaAttr);

                    if (ModMethodInfo.ReturnType != typeof(void))
                       LuaFunc.m_ReturnType = ModMethodInfo.ReturnType;

                    ParameterInfo[] MethodParams = ModMethodInfo.GetParameters();

                    foreach (ParameterInfo ParamInfo in MethodParams)
                    {
                        Type ParamType = ParamInfo.ParameterType;

                        if (!Lua.AllowedLuaTypes.Contains(ParamType))
                            throw new Exception(string.Format("Invalid parameter for lua function (name: {0} - type: {1})", ParamInfo.Name, ParamInfo.ParameterType));

                        LuaFunc.m_Args.Add(ParamType); 
                    }

                    lock(LuaFunctions)
                        LuaFunctions[LuaFunc.m_FunctionName] = LuaFunc;

                    _RegisterLuaFunctionInternal(LuaAttr.m_FunctionName);
                }
            }
        }

        /*
         * removes all the functions belonging to a mod
         */
        internal static void UnregisterLuaFunctions(ModManager.ModContext Mod)
        {
            lock(LuaFunctions)
            {
                foreach (KeyValuePair<string, RegisteredLuaFunction> Pair in LuaFunctions)
                {
                    if (Pair.Value.m_Context == Mod)
                    {
                        LuaFunctions.Remove(Pair.Key);
                        _UnregisterLuaFunctionInternal(Pair.Key);
                    }
                }
            }
        }
    }
}