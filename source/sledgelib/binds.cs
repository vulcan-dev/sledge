using System.Collections.Concurrent;

internal delegate void dInputReader(EKeyCode iKeyCode, bool bKeyDown);

internal class CBindManager
{
    internal static ConcurrentBag<CBind> m_Binds = new ConcurrentBag<CBind>();

    internal static void OnInputReader(EKeyCode iKeyCode, bool bKeyDown)
    {
        // haven't implemented other bind types yet
        if (!bKeyDown)
            return;

        foreach (CBind Bind in m_Binds)
        {
            if (!Bind.m_Active)
                continue;

            if (Bind.m_KeyCode != iKeyCode)
                continue;

            try
            {
                Bind.m_Callback();
            } catch(Exception e)
            {
                SledgeLib.WriteError("Bind error: " + e.Message);
                Bind.m_Active = false;
            }
        }
    }

    internal static dInputReader InputReader = new dInputReader(OnInputReader);
}

public class CBind
{
    internal EKeyCode m_KeyCode;
    internal dCallback m_Callback;
    public bool m_Active;

    public CBind(EKeyCode eKey, dCallback Callback, bool bActive = true)
    {
        m_KeyCode = eKey;
        m_Callback = Callback;
        m_Active = bActive;

        CBindManager.m_Binds.Add(this);
    }
}