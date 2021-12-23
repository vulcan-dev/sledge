using System.Runtime.InteropServices;

public class SledgeLib
{
    #pragma warning disable 8618
    [StructLayout(LayoutKind.Explicit, Pack = 1, Size = 8*4)]
    internal class CSledgeCAPI
    {
        internal delegate void dLog(ELogType LogType, string sMsg);
        [FieldOffset(0x0)] internal dLog Log;

        internal delegate IntPtr dRegisterInputReader(dInputReader Reader);
        [FieldOffset(0x8)] internal dRegisterInputReader RegisterInputReader;

        internal delegate IntPtr dRegisterCallback(ECallbackType eType, dCallback pCallback);
        [FieldOffset(0x10)] internal dRegisterCallback RegisterCallback;

        internal delegate IntPtr dGetPlayer();
        [FieldOffset(0x18)] internal dGetPlayer GetPlayer;
    }
    
    internal static CSledgeCAPI m_CAPI;
    internal static CPlayerWrapper m_PlayerWrapper;
    #pragma warning restore 8618

    public static CPlayer Player = new CPlayer();

    internal static bool _InitCAPI(IntPtr pAPI)
    {
        object? CAPI = Marshal.PtrToStructure(pAPI, typeof(CSledgeCAPI));
        if (CAPI == null)
            return false;
        m_CAPI = (CSledgeCAPI)CAPI;

        object? PlayerWrapper = Marshal.PtrToStructure(m_CAPI.GetPlayer(), typeof(CPlayerWrapper));
        if (PlayerWrapper == null)
            return false;
        m_PlayerWrapper = (CPlayerWrapper)PlayerWrapper;

        m_CAPI.RegisterInputReader(CBindManager.InputReader);

        m_CAPI.RegisterCallback(ECallbackType.PlayerSpawn, CCallbackManager.PlayerSpawnCallback);
        m_CAPI.RegisterCallback(ECallbackType.PreUpdate, CCallbackManager.PreUpdateCallback);
        m_CAPI.RegisterCallback(ECallbackType.PostUpdate, CCallbackManager.PostUpdateCallback);

        WriteLog("sledgelib loaded");
        return true;
    }

    internal enum ELogType
    {
        General = 4,
        Error = 5
    }

    public static void WriteLog(string sLog)
    {
        m_CAPI.Log(ELogType.General, sLog);
    }

    public static void WriteError(string sLog)
    {
        m_CAPI.Log(ELogType.Error, sLog);
    }
}