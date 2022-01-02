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
    
    public enum EProjectileType
    {
        Normal = 0,
        Shotgun = 1,
        Rocket = 2
    }

    public class Scene
    {
        [DllImport("sledge.dll")] public static extern SRaycastReturn QueryRaycast(Vector3 vOrigin, Vector3 vDirection, float fMaxDist);
        [DllImport("sledge.dll")] public static extern void Shoot(Vector3 vOrigin, Vector3 vVelocity, EProjectileType eProjectileType, float fLifeTime, float fDamage);
        [DllImport("sledge.dll")] public static extern void DrawLine(Vector3 vPoint0, Vector3 vPoint1, Vector4 vColor, bool bIgnoreDepth);
    }
}