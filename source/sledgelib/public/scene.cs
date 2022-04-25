using System.Numerics;
using System.Runtime.InteropServices;

namespace SledgeLib
{

    public class Scene
    {
        [DllImport("sledge_core.dll")] public static extern RaycastInfo QueryRaycast(Vector3 vOrigin, Vector3 vDirection, float fMaxDist);
    }
}