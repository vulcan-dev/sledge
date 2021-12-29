using System.Runtime.InteropServices;
using System.Reflection;

namespace SledgeLib
{
    public class Log
    {
        [DllImport("sledge.dll")]
        private static extern void _WriteLog(ELogType eLogtype, string cMsg);

        internal enum ELogType
        {
            General = 4,
            Warning = 5,
            Error = 6
        }

        private static void _Log(ELogType eType, Assembly Caller, string sFormat, params object[] oArgs)
        {
            string sCallingAssembly = "unknown";
            string? sAssemblyName = Caller.GetName().Name;
            if (Caller != null && sAssemblyName != null)
                sCallingAssembly = sAssemblyName;

            string sStackTrace = Environment.StackTrace;

            string sMsg = "";
            try
            {
                sMsg = string.Format(sFormat, oArgs);
            }
            catch (Exception e)
            {
                Error("Logger error while formatting: {0}", e.Message);
                Error("Stack trace: {0}", sStackTrace);
                return;
            }

            _WriteLog(eType, "[" + sCallingAssembly + "] - " + sMsg);
        }

        public static void General(string sFormat, params object[] oArgs) { _Log(ELogType.General, Assembly.GetCallingAssembly(), sFormat, oArgs); }

        public static void Warning(string sFormat, params object[] oArgs) { _Log(ELogType.Warning, Assembly.GetCallingAssembly(), sFormat, oArgs); }

        public static void Error(string sFormat, params object[] oArgs) { _Log(ELogType.Error, Assembly.GetCallingAssembly(), sFormat, oArgs); }
    }
}