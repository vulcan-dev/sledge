using System.Numerics;
using System.Runtime.InteropServices;

namespace SledgeLib
{
    [StructLayout(LayoutKind.Explicit)]
    public struct QueryInfo
    {
        [FieldOffset(0x0)]  public bool Hit; // True if the query was succesful
        [FieldOffset(0x4)]  public float Distance; // The distance to the point hit
        [FieldOffset(0x8)]  public Vector3 Normal; // The normal from the origin to that point (Only used on QueryRaycast and QueryClosestPoint)
        [FieldOffset(0x14)]  public Vector3 Position; // The position where the query hit
        [FieldOffset(0x20)]  public Vector3 Angle; // The angle in which the sphere was hit (Only used on QuerySpherecast)
        [FieldOffset(0x2C)] public uint Shape; // The id / handle of the shape that was hit
    }
}