public delegate void BindCallback();

internal struct SBindWrapper
{
    IntPtr m_BindInstance;

    internal delegate void DestroyDelegate(SBindWrapper pThis);
    internal DestroyDelegate Destroy;

    internal delegate void UpdateActiveDelegate(SBindWrapper pThis, bool bActive);
    internal UpdateActiveDelegate UpdateActive;
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

    public CBind(EBindType eType, EKeyCode iKeyCode, BindCallback pCallback, bool bActive = true)
    {
        IntPtr pBindWrapper = SledgeLib.m_Internal.CreateBind(eType, iKeyCode, pCallback, bActive); ;

        if (pBindWrapper == IntPtr.Zero)
            return;

        SBindWrapper? NewBindWrapper = (SBindWrapper)System.Runtime.InteropServices.Marshal.PtrToStructure(pBindWrapper, typeof(SBindWrapper));
        if (NewBindWrapper == null)
            return;

        m_BindWrapper = (SBindWrapper)NewBindWrapper;
    }

    ~CBind()
    {
        this.m_BindWrapper.Destroy(this.m_BindWrapper);
    }

    public void UpdateActive(bool bActive)
    {
        this.m_BindWrapper.UpdateActive(this.m_BindWrapper, bActive);
    }
}
