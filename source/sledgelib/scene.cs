using System.Runtime.InteropServices;
using System.Reflection;
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

    [StructLayout(LayoutKind.Explicit)]
    public struct SParticleParamV4
    {
        [FieldOffset(0x0)] public EParticleInterpolation m_InterpolationType;
        [FieldOffset(0x4)] public float m_FadeIn;
        [FieldOffset(0x8)] public float m_FadeOut;
        [FieldOffset(0xC)] public Vector4 m_Start;
        [FieldOffset(0x1C)] public Vector4 m_End;
        public SParticleParamV4()
        {
            m_InterpolationType = 0;
            m_FadeIn = 0;
            m_FadeOut = 1;
            m_Start = new Vector4(1, 1, 1, 1);
            m_End = new Vector4(0, 0, 0, 1);
        }

    }

    [StructLayout(LayoutKind.Explicit)]
    public struct SParticleParamF
    {

        [FieldOffset(0x0)] public EParticleInterpolation m_InterpolationType;
        [FieldOffset(0x4)] public float m_FadeIn;
        [FieldOffset(0x8)] public float m_FadeOut;
        [FieldOffset(0xC)] public float m_Start;
        [FieldOffset(0x10)] public float m_End;
        public SParticleParamF()
        {
            m_InterpolationType = 0;
            m_FadeIn = 0;
            m_FadeOut = 1;
            m_Start = 0;
            m_End = 0;
        }
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct SParticleInfo
    {
        [FieldOffset(0x0)] public SParticleParamV4 m_Color;
        [FieldOffset(0x2C)] public SParticleParamF m_Alpha;
        [FieldOffset(0x40)] public SParticleParamF m_Radius;
        [FieldOffset(0x54)] public SParticleParamF m_Gravity;
        [FieldOffset(0x68)] public SParticleParamF m_Drag;
        [FieldOffset(0x7C)] public SParticleParamF m_Emissive;
        [FieldOffset(0x90)] public SParticleParamF m_Rotation;
        [FieldOffset(0xA4)] public SParticleParamF m_Collide;
        [FieldOffset(0xB8)] public SParticleParamF m_Stretch;
        [FieldOffset(0xCC)] public SParticleParamF m_Sticky;
        [FieldOffset(0xE0)] public uint m_Tile;
        [FieldOffset(0xE4)] public uint m_Flags;

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

        [DllImport("sledge.dll")] private static extern uint _LoadSound(string sSoundPath);
        public static uint LoadSound(string sSoundPath)
        {
            string? sVerifiedPath = CSledgeUtils.GetValidPath(sSoundPath, Assembly.GetCallingAssembly());
            if (sVerifiedPath == null)
                throw new Exception("unable to resolve path");

            return _LoadSound("RAW:" + sVerifiedPath.Replace("\\", "/"));
        }

        [DllImport("sledge.dll")] private static extern uint _LoadLoop(string sLoopPath);
        public static uint LoadLoop(string sLoopPath)
        {
            string? sVerifiedPath = CSledgeUtils.GetValidPath(sLoopPath, Assembly.GetCallingAssembly());
            if (sVerifiedPath == null)
                throw new Exception("unable to resolve path");

            return _LoadLoop("RAW:" + sVerifiedPath.Replace("\\", "/"));
        }

        [DllImport("sledge.dll")] public static extern void PlaySound(uint iSoundHandle, Vector3 vPosition, float fVolume, float fSpeed);
        [DllImport("sledge.dll")] public static extern void PlayLoop(uint iLoopHandle, Vector3 vPosition, float fVolume, bool bSomething);

        [DllImport("sledge.dll")] internal static extern void _ResetSounds();

        [DllImport("sledge.dll")] public static extern void SpotLight(Vector3 vStartPos, Vector3 vColor, float fRayDispersion, float fRadius, float fFog);
    }
}