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

    public enum EParticleInterpolation
    {
        Linear = 0,
        Smooth = 1,
        EaseIn = 2,
        Constant = 3,
        EaseOut = 4
    }

    public struct SParticleParamV4
    {
        public EParticleInterpolation m_InterpolationType;
        public float m_FadeIn;
        public float m_FadeOut;
        public Vector4 m_Start;
        public Vector4 m_End;
        public SParticleParamV4()
        {
            m_InterpolationType = 0;
            m_FadeIn = 0;
            m_FadeOut = 1;
            m_Start = new Vector4(1, 1, 1, 1);
            m_End = new Vector4(0, 0, 0, 1);
        }

    }

    public struct SParticleParamF
    {
        public EParticleInterpolation m_InterpolationType;
        public float m_FadeIn;
        public float m_FadeOut;
        public float m_Start;
        public float m_End;
        public SParticleParamF()
        {
            m_InterpolationType = 0;
            m_FadeIn = 0;
            m_FadeOut = 1;
            m_Start = 0;
            m_End = 0;
        }
    }

    public struct SParticleInfo
    {
        public SParticleParamV4 m_Color;
        public SParticleParamF m_Alpha;
        public SParticleParamF m_Radius;
        public SParticleParamF m_Gravity;
        public SParticleParamF m_Drag;
        public SParticleParamF m_Emissive;
        public SParticleParamF m_Rotation;
        public SParticleParamF m_Collide;
        public SParticleParamF m_Stretch;
        public SParticleParamF m_Sticky;
        public uint m_Tile;
        public uint m_Flags;

        public SParticleInfo()
        {
            m_Color = new SParticleParamV4();
            m_Alpha = new SParticleParamF();
            m_Radius = new SParticleParamF();
            m_Gravity = new SParticleParamF();
            m_Drag = new SParticleParamF();
            m_Emissive = new SParticleParamF();
            m_Rotation = new SParticleParamF();
            m_Collide = new SParticleParamF();
            m_Stretch = new SParticleParamF();
            m_Sticky = new SParticleParamF();

            m_Alpha.m_Start = 1.0f;
            m_Alpha.m_InterpolationType = EParticleInterpolation.Smooth;

            m_Radius.m_Start = 0.5f;
            m_Radius.m_End = 0.5f;

            m_Collide.m_Start = 1.0f;
            m_Collide.m_End = 1.0f;

            m_Stretch.m_Start = 1.0f;
            m_Stretch.m_End = 1.0f;

            m_Tile = 0;
            m_Flags = 0;
        }
    }
    
    public class Scene
    {
        [DllImport("sledge.dll")] public static extern SRaycastReturn QueryRaycast(Vector3 vOrigin, Vector3 vDirection, float fMaxDist);
        [DllImport("sledge.dll")] public static extern void Shoot(Vector3 vOrigin, Vector3 vVelocity, EProjectileType eProjectileType, float fLifeTime, float fDamage);
        [DllImport("sledge.dll")] public static extern void DrawLine(Vector3 vPoint0, Vector3 vPoint1, Vector4 vColor, bool bIgnoreDepth);

        [DllImport("sledge.dll")] public static extern void SpawnParticle(SParticleInfo ParticleInfo, Vector3 vSpawnPos, Vector3 vVelocity, float fLifeTime);
    }
}