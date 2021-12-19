internal struct SBindWrapper
{
    IntPtr m_BindInstance;

    internal delegate void DestroyDelegate(SBindWrapper pThis);
    internal DestroyDelegate Destroy;

    internal delegate void SetActiveDelegate(SBindWrapper pThis, bool bActive);
    internal SetActiveDelegate SetActive;
}

public class CBind
{
    internal SBindWrapper m_BindWrapper;

    public CBind(EKeyCode iKeyCode, CallbackDelegate pCallback, bool bActive = true)
    {
        IntPtr pBindWrapper = SledgeLib.m_Internal.CreateBind(iKeyCode, pCallback, bActive); ;

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
