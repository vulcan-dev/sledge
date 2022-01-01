using System.Runtime.InteropServices;
using System.Numerics;

namespace SledgeLib
{
    public struct SRaycastReturn
    {
        public bool m_Hit;
        public float m_HitDist;
        public Vector3 m_HitNormal;
        public uint m_HitShape;
    }

    public class Scene
    {
        [DllImport("sledge.dll")] public static extern SRaycastReturn QueryRaycast(Vector3 vOrigin, Vector3 vDirection, float fMaxDist);
    }
}