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
        string m_FunctionName;

        public LuaFunction(string FunctionName)
        {
            m_FunctionName = FunctionName;
        }
    }

    internal class Lua
    {
        [DllImport("sledge_core.dll")]
        internal static extern int _lua_tointeger(IntPtr pLuaState, int Index);
        [DllImport("sledge_core.dll")]
        internal static extern bool _lua_toboolean(IntPtr pLuaState, int Index);
        [DllImport("sledge_core.dll")]
        internal static extern double _lua_tonumber(IntPtr pLuaState, int Index);
        [DllImport("sledge_core.dll")]
        internal static extern StringBuilder _lua_tolstring(IntPtr pLuaState, int Index);

        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pushnil(IntPtr pLuaState);
        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pushnumber(IntPtr pLuaState, double Value);
        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pushinteger(IntPtr pLuaState, int Value);
        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pushboolean(IntPtr pLuaState, bool Value);
        [DllImport("sledge_core.dll")]
        internal static extern void _lua_pushlstring(IntPtr pLuaState, string Value);
    }

    internal class LuaFunctionManager
    {
        internal delegate int dLuaFunction(IntPtr pScriptCore, IntPtr pLuaState, string FunctionName);

        [DllImport("sledge_core.dll")]
        internal static extern void _RegisterLuaFunctionInternal(string FunctionName, dLuaFunction Function);

        internal struct RegisteredLuaFunction
        {
            public string m_FunctionName;
            public MethodInfo m_Method;
            public ModManager.ModContext m_Context;
            public Type m_ReturnType;
            public List<Type> m_Args;

            public RegisteredLuaFunction(MethodInfo Method, ModManager.ModContext Ctx)
            {
                m_Args = new List<Type>();
                m_ReturnType = Method.ReturnType;
                m_FunctionName = Method.Name;
                m_Method = Method;
                m_Context = Ctx;
            }
        }

        internal static List<Type> AllowedLuaTypes = new List<Type>()
        {
            typeof(int), typeof(string), typeof(bool), typeof(float), typeof(double)
        };

        internal static Dictionary<string, RegisteredLuaFunction> LuaFunctions = new Dictionary<string, RegisteredLuaFunction>();

        /*
         * function wrapper in charge of parsing lua arguments, invoking the C# method and returns
         */
        private static int LuaFunctionWrapper(IntPtr pSC, IntPtr pL, string FunctionName)
        {
            if (!LuaFunctions.ContainsKey(FunctionName))
                return 0;

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

                    RegisteredLuaFunction LuaFunc = new RegisteredLuaFunction(ModMethodInfo, Ctx);

                    if (ModMethodInfo.ReturnType != typeof(void))
                       LuaFunc.m_ReturnType = ModMethodInfo.ReturnType;

                    ParameterInfo[] MethodParams = ModMethodInfo.GetParameters();

                    foreach (ParameterInfo ParamInfo in MethodParams)
                    {
                        Type ParamType = ParamInfo.ParameterType;

                        if (!AllowedLuaTypes.Contains(ParamType))
                            throw new Exception(string.Format("Invalid parameter for lua function (name: {0} - type: {1})", ParamInfo.Name, ParamInfo.ParameterType));

                        LuaFunc.m_Args.Add(ParamType); 
                    }

                    lock(LuaFunctions)
                        LuaFunctions[LuaFunc.m_FunctionName] = LuaFunc;

                    _RegisterLuaFunctionInternal(LuaFunc.m_FunctionName, LuaFunctionWrapper);
                }
            }
        }

        /*
         * removes all the functions belonging to a mod
         */

        internal static void UnregisterLuaFunctions(ModManager.ModContext Mod)
        {
            lock(LuaFunctions)
                foreach (KeyValuePair<string, RegisteredLuaFunction> Pair in LuaFunctions)
                    if (Pair.Value.m_Context == Mod)
                        LuaFunctions.Remove(Pair.Key);
        }
    }
}