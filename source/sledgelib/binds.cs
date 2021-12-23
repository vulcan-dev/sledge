using System.Collections.Concurrent;

internal delegate void dInputReader(EKeyCode iKeyCode, bool bKeyDown);

public delegate void dBindCallback();
public delegate void dAdvancedBindCallback(bool bKeyDown);

internal class CBindManager
{
    internal static ConcurrentBag<CBind> m_Binds = new ConcurrentBag<CBind>();
    internal static ConcurrentBag<CAdvancedBind> m_AdvancedBinds = new ConcurrentBag<CAdvancedBind>();

    internal static void OnInputReader(EKeyCode iKeyCode, bool bKeyDown)
    {
        foreach (CBind Bind in m_Binds)
        {
            if (!Bind.m_Active)
                continue;

            if (Bind.m_KeyCode != iKeyCode)
                continue;

            if (!bKeyDown)
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

        foreach (CAdvancedBind AdvBind in m_AdvancedBinds)
        {
            if (!AdvBind.m_Active)
                continue;

            if (AdvBind.m_KeyCode != iKeyCode)
                continue;

            try
            {
                AdvBind.m_Callback(bKeyDown);
            }
            catch (Exception e)
            {
                SledgeLib.WriteError("Bind error: " + e.Message);
                AdvBind.m_Active = false;
            }
        }
    }

    internal static dInputReader InputReader = new dInputReader(OnInputReader);
}

public class CBind
{
    internal EKeyCode m_KeyCode;
    internal dBindCallback m_Callback;
    public bool m_Active;
    public CBind(EKeyCode eKey, dBindCallback Callback, bool bActive = true)
    {
        m_KeyCode = eKey;
        m_Callback = Callback;
        m_Active = bActive;

        CBindManager.m_Binds.Add(this);
    }
}

public class CAdvancedBind
{
    internal EKeyCode m_KeyCode;
    internal dAdvancedBindCallback m_Callback;
    public bool m_Active;

    public CAdvancedBind(EKeyCode eKey, dAdvancedBindCallback Callback, bool bActive = true)
    {
        m_KeyCode = eKey;
        m_Callback = Callback;
        m_Active = bActive;

        CBindManager.m_AdvancedBinds.Add(this);
    }
}