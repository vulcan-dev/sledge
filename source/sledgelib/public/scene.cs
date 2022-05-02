using System.Numerics;
using System.Runtime.InteropServices;

namespace SledgeLib
{
    public class Scene
    {
        [DllImport("sledge_core.dll")] public static extern QueryInfo QueryRaycast(Vector3 Origin, Vector3 Direction, float MaxDist);
        [DllImport("sledge_core.dll")] public static extern QueryInfo QuerySpherecast(Vector3 Origin, Vector3 Direction, float Radius, float MaxDist);
        [DllImport("sledge_core.dll")] public static extern QueryInfo QueryClosestPoint(Vector3 Origin, float MaxDist);
        [DllImport("sledge_core.dll")] public static extern uint MakeHole(Vector3 Position, float SoftRadius, float MidRadius, float HardRadius, bool Silent);
        [DllImport("sledge_core.dll")] public static extern void DrawLine(Vector3 Origin, Vector3 Goal, Vector4 Colour, bool UseDepth);
        [DllImport("sledge_core.dll")] public static extern void DrawCross(Vector3 Position, Vector4 Colour, float Size);
    }
}