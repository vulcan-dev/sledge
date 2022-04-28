using System.Numerics;
using System.Runtime.InteropServices;

namespace SledgeLib
{

    public class Scene
    {
        [DllImport("sledge_core.dll")] public static extern RaycastInfo QueryRaycast(Vector3 Origin, Vector3 Direction, float MaxDist);
        [DllImport("sledge_core.dll")] public static extern uint MakeHole(Vector3 Position, float SoftRadius, float MidRadius, float HardRadius, bool Silent);
    }
}