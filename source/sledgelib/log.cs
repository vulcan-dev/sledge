using System.Runtime.InteropServices;
using System.Reflection;
using System;

namespace SledgeLib
{
    public class Log
    {
        internal enum ELogType
        {
            General = 4,
            Warning = 5,
            Error = 6,
            Verbose = 7
        }

        [DllImport("sledge_core.dll")]
        private static extern void _WriteLog(ELogType eLogtype, string cMsg);

        private static void _Log(ELogType eType, Assembly Caller, string sFormat, params object[] oArgs)
        {
            string sCallingAssembly = "unknown";
            string? sAssemblyName = Caller.GetName().Name;
            if (Caller != null && sAssemblyName != null)
                sCallingAssembly = sAssemblyName;

            string sMsg;
            try
            {
                sMsg = string.Format(sFormat, oArgs);
            }
            catch (Exception e)
            {
                Error("Logger error while formatting: {0} \n{1}", e.Message, Environment.StackTrace);
                return;
            }
            _WriteLog(eType, "[" + sCallingAssembly + "] - " + sMsg);
        }

        public static void General(string sFormat, params object[] oArgs) { _Log(ELogType.General, Assembly.GetCallingAssembly(), sFormat, oArgs); }
        public static void Warning(string sFormat, params object[] oArgs) { _Log(ELogType.Warning, Assembly.GetCallingAssembly(), sFormat, oArgs); }
        public static void Error(string sFormat, params object[] oArgs) { _Log(ELogType.Error, Assembly.GetCallingAssembly(), sFormat, oArgs); }
        public static void Verbose(string sFormat, params object[] oArgs) { _Log(ELogType.Verbose, Assembly.GetCallingAssembly(), sFormat, oArgs); }

    }
}