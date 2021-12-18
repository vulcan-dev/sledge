public enum ECallbackType : uint
{
    PlayerSpawn = 0,
    PreUpdate = 1,
    PostUpdate = 2
}

internal struct SCallbackWrapper
{
    IntPtr m_CallbackInstance;

    internal delegate void DestroyDelegate(SCallbackWrapper pThis);
    internal DestroyDelegate Destroy;

    internal delegate void SetActiveDelegate(SCallbackWrapper pThis, bool bActive);
    internal SetActiveDelegate SetActive;
}

public class CCallback
{
    private SCallbackWrapper m_CallbackWrapper;

    public CCallback(ECallbackType eType, CallbackDelegate pCallback, bool bActive = true)
    {
        IntPtr pCallbackWrapper = SledgeLib.m_Internal.CreateCallback(eType, pCallback, bActive);

        if (pCallbackWrapper == IntPtr.Zero)
            return;

        SCallbackWrapper? NewCallbackWrapper = (SCallbackWrapper?)System.Runtime.InteropServices.Marshal.PtrToStructure(pCallbackWrapper, typeof(SCallbackWrapper));
        if (NewCallbackWrapper == null)
            return;

        m_CallbackWrapper = (SCallbackWrapper)NewCallbackWrapper;
    }

    ~CCallback()
    {
        this.m_CallbackWrapper.Destroy(this.m_CallbackWrapper);
    }

    public void SetActive(bool bActive)
    {
        this.m_CallbackWrapper.SetActive(this.m_CallbackWrapper, bActive);
    }
}