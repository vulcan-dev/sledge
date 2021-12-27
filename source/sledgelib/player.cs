using System.Numerics;
using System.Runtime.InteropServices;

#pragma warning disable 8618
[StructLayout(LayoutKind.Explicit, Pack = 1, Size = 8 * 10)]
internal class CPlayerWrapper
{
    internal delegate void dSetFloat(float fValue);
    internal delegate float dGetFloat();

    internal delegate Vector2 dGetVector2();
    internal delegate void dSetVector2(Vector2 vValue);

    internal delegate Vector3 dGetVector3();
    internal delegate void dSetVector3(Vector3 vValue);

    internal delegate Transform dGetTransform();
    internal delegate void dSetTransform(Transform tValue);

    [FieldOffset(0x0)] internal dSetFloat SetHealth;
    [FieldOffset(0x8)] internal dGetFloat GetHealth;

    [FieldOffset(0x10)] internal dSetVector3 SetPosition;
    [FieldOffset(0x18)] internal dGetVector3 GetPosition;

    [FieldOffset(0x20)] internal dSetVector3 SetVelocity;
    [FieldOffset(0x28)] internal dGetVector3 GetVelocity;

    [FieldOffset(0x30)] internal dSetTransform SetCameraTransform;
    [FieldOffset(0x38)] internal dGetTransform GetCameraTransform;

    [FieldOffset(0x40)] internal dGetVector2 GetMovementKeys;
    [FieldOffset(0x48)] internal dGetVector2 GetMouseInput;
}
#pragma warning restore 8618

public class CPlayer
{
    public float m_Health
    {
        get { return SledgeLib.m_PlayerWrapper.GetHealth(); }
        set { SledgeLib.m_PlayerWrapper.SetHealth(value); }
    }

    public Vector3 m_Position
    {
        get { return SledgeLib.m_PlayerWrapper.GetPosition(); }
        set { SledgeLib.m_PlayerWrapper.SetPosition(value); }
    }

    public Vector3 m_Velocity
    {
        get { return SledgeLib.m_PlayerWrapper.GetVelocity(); }
        set { SledgeLib.m_PlayerWrapper.SetVelocity(value); }
    }

    public Transform m_CameraTransform
    {
        get { return SledgeLib.m_PlayerWrapper.GetCameraTransform(); }
        set { SledgeLib.m_PlayerWrapper.SetCameraTransform(value); }
    }

    public Vector2 m_MovementKeys
    {
        get { return SledgeLib.m_PlayerWrapper.GetMovementKeys(); }
    }

    public Vector2 m_MouseInput
    {
        get { return SledgeLib.m_PlayerWrapper.GetMouseInput(); }
    }
}