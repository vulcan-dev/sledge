internal struct SPlayerWrapper
{
    internal delegate float GetHealthDelegate();
    internal GetHealthDelegate GetHealth;

    internal delegate void SetHealthDelegate(float fHealth);
    internal SetHealthDelegate SetHealth;

    internal delegate Vector3 GetPositionDelegate();
    internal GetPositionDelegate GetPosition;

    internal delegate void SetPositionDelegate(Vector3 vPosition);
    internal SetPositionDelegate SetPosition;
}

public interface Player
{
    internal static SPlayerWrapper m_PlayerWrapper;

    internal static void SetWrapper(IntPtr pPlayerWrapper)
    {
        if (pPlayerWrapper == IntPtr.Zero)
            return;

        SPlayerWrapper? NewPlayerWrapper = (SPlayerWrapper?)System.Runtime.InteropServices.Marshal.PtrToStructure(pPlayerWrapper, typeof(SPlayerWrapper));
        
        if (NewPlayerWrapper == null)
            return;

        m_PlayerWrapper = (SPlayerWrapper)NewPlayerWrapper;
    }

    public static float m_Health
    {
        get { return m_PlayerWrapper.GetHealth(); }
        set { m_PlayerWrapper.SetHealth(value);}
    }

    public static Vector3 m_Position
    {
        get {  return m_PlayerWrapper.GetPosition(); }
        set { m_PlayerWrapper.SetPosition(value); }
    }
}