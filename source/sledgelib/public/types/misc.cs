using System.Numerics;
using System.Runtime.InteropServices;

namespace SledgeLib
{
    [StructLayout(LayoutKind.Explicit)]
    public struct QueryInfo
    {
        [FieldOffset(0x0)]  public bool Hit;
        [FieldOffset(0x4)]  public float HitDist;
        [FieldOffset(0x8)]  public Vector3 HitNormal;
        [FieldOffset(0x14)]  public Vector3 HitPos;
        [FieldOffset(0x20)] public uint HitShape;
    }
}