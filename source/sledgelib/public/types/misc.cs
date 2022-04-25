using System.Numerics;
using System.Runtime.InteropServices;

namespace SledgeLib
{
    [StructLayout(LayoutKind.Explicit)]
    public struct RaycastInfo
    {
        [FieldOffset(0x0)]  public bool Hit;
        [FieldOffset(0x4)]  public float HitDist;
        [FieldOffset(0x8)]  public Vector3 HitNormal;
        [FieldOffset(0x14)] public uint HitShape;
    }

}