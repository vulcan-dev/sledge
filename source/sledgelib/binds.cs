internal struct SBindWrapper
{
    IntPtr m_BindInstance;

    internal delegate void DestroyDelegate(SBindWrapper pThis);
    internal DestroyDelegate Destroy;

    internal delegate void SetActiveDelegate(SBindWrapper pThis, bool bActive);
    internal SetActiveDelegate SetActive;
}

/*
    to-do: port over the other bind types
 */
public enum EBindType : uint
{
    Callback = 0
}

public class CBind
{
    private SBindWrapper m_BindWrapper;

    public CBind(EBindType eType, EKeyCode iKeyCode, CallbackDelegate pCallback, bool bActive = true)
    {
        IntPtr pBindWrapper = SledgeLib.m_Internal.CreateBind(eType, iKeyCode, pCallback, bActive); ;

        if (pBindWrapper == IntPtr.Zero)
            return;

        SBindWrapper? NewBindWrapper = (SBindWrapper?)System.Runtime.InteropServices.Marshal.PtrToStructure(pBindWrapper, typeof(SBindWrapper));
        if (NewBindWrapper == null)
            return;

        m_BindWrapper = (SBindWrapper)NewBindWrapper;
    }

    ~CBind()
    {
        this.m_BindWrapper.Destroy(this.m_BindWrapper);
    }

    public void SetActive(bool bActive)
    {
        this.m_BindWrapper.SetActive(this.m_BindWrapper, bActive);
    }
}
