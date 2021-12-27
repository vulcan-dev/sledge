using System.Runtime.InteropServices;
using System.Reflection;

namespace SledgeLib
{
    public class Log
    {
        [DllImport("sledge.dll")]
        private static extern void WriteLog(ELogType eLogtype, string cMsg);

        internal enum ELogType
        {
            General = 4,
            Error = 5
        }

        public static void General(string sFormat, params object[] oArgs)
        {
            Assembly Caller = Assembly.GetCallingAssembly();
            string sCallingAssembly = "unknown";
            string? sAssemblyName = Caller.GetName().Name;
            if (Caller != null && sAssemblyName != null)
                sCallingAssembly = sAssemblyName;

            string sMsg = "";
            try
            {
                sMsg = string.Format(sFormat, oArgs);
            }
            catch (Exception e)
            {
                Error("Logger error while formatting: {0}", e.Message);
            }
            WriteLog(ELogType.General, "[" + sCallingAssembly + "] - " + sMsg);
        }

        public static void Error(string sFormat, params object[] oArgs)
        {
            Assembly Caller = Assembly.GetCallingAssembly();
            string sCallingAssembly = "unknown";
            string? sAssemblyName = Caller.GetName().Name;
            if (Caller != null && sAssemblyName != null)
                sCallingAssembly = sAssemblyName;

            string sMsg = "";
            try
            {
                sMsg = string.Format(sFormat, oArgs);
            } catch (Exception e)
            {
                Error("Logger error while formatting: {0}", e.Message);
            }
            WriteLog(ELogType.Error, "[" + sCallingAssembly + "] - " + sMsg);
        }
    }
}