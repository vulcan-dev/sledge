using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;

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
        internal static extern StringBuilder _lua_tolstring(IntPtr pLuaState, int Index);
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
            public List<Type> m_Args;

            public RegisteredLuaFunction(MethodInfo Method, ModManager.ModContext Ctx)
            {
                m_Args = new List<Type>();
                m_FunctionName = Method.Name;
                m_Method = Method;
                m_Context = Ctx;
            }
        }

        internal static List<Type> AllowedLuaTypes = new List<Type>()
        {
            typeof(int), typeof(string), typeof(bool)/*, typeof(float), typeof(double)*/
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
                        default:
                            Log.Error("Unknown type: {0}", ArgumentType.Name);
                            return 0;
                    }
                    Index++;
                }

                try
                {
                    LuaFunc.m_Method.Invoke(null, Args.ToArray());
                }
                catch (Exception e)
                {
                    Log.Error("Error on lua function {0}: {1}", LuaFunc.m_FunctionName, e);
                    return 0;
                }
            }
            else
            {
                try
                {
                    LuaFunc.m_Method.Invoke(null, null);
                }
                catch (Exception e)
                {
                    Log.Error("Error on lua function {0}: {1}", LuaFunc.m_FunctionName, e);
                    return 0;
                }
            }

            return 0;
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

                    if (ModMethodInfo.ReturnType != typeof(void))
                        throw new Exception("return types other than void are not implemented yet");

                    RegisteredLuaFunction LuaFunc = new RegisteredLuaFunction(ModMethodInfo, Ctx);

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

            /*
             * TO-DO: iterate every state and unregister it from there, also remove it from vFunctions @ luahelpers.cpp
             */
        }
    }
}