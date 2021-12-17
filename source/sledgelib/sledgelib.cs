public class SledgeLib
{
    private enum ELogType
    {
        General = 4,
        Error = 5
    }

    private struct CSledgeInternal
    {
        public delegate void LogDelegate(ELogType LogType, string sMsg);
        public LogDelegate Log;
    }

    private static CSledgeInternal _m_Internal;

    internal static bool _SetInternal(IntPtr pInternal)
    {
        if (pInternal == IntPtr.Zero)
            return false;

        object? InternalObj = System.Runtime.InteropServices.Marshal.PtrToStructure(pInternal, typeof(CSledgeInternal));
        if (InternalObj == null)
            return false;

        _m_Internal = (CSledgeInternal)InternalObj;
        WriteLog("sledgelib internal api loaded");

        return true;
    }

    public static void WriteLog(string sLog)
    {
        _m_Internal.Log(ELogType.General, sLog);
    }

    public static void WriteError(string sLog)
    {
        _m_Internal.Log(ELogType.Error, sLog);
    }
}