using System.Runtime.InteropServices;
using System.Numerics;
using System.Reflection;

namespace SledgeLib
{
    public class CShape
    {
        [DllImport("sledge.dll")] private static extern uint Shape_Create(uint iBodyHandle);
        [DllImport("sledge.dll")] private static extern void Shape_Destroy(uint iHandle);

        [DllImport("sledge.dll")] private static extern Transform Shape_GetLocalTransform(uint iHandle);
        [DllImport("sledge.dll")] private static extern void Shape_SetLocalTransform(uint iHandle, Transform tTransform);

        [DllImport("sledge.dll")] private static extern float Shape_GetEmissiveScale(uint iHandle);
        [DllImport("sledge.dll")] private static extern void Shape_SetEmissiveScale(uint iHandle, float fValue);

        [DllImport("sledge.dll")] private static extern Bounds Shape_GetBounds(uint iHandle);

        [DllImport("sledge.dll")] private static extern bool Shape_IsBroken(uint iHandle);

        [DllImport("sledge.dll")] private static extern void Shape_SetCollisionFilter(uint iHandle, sbyte CollisionLayer, sbyte CollisionMask);

        [DllImport("sledge.dll")] private static extern bool Shape_LoadVox(uint iHandle, string sVoxPath, string sVoxName, float fScale);

        public readonly uint m_Handle = 0;
        public Transform m_LocalTransform
        {
            get { return Shape_GetLocalTransform(m_Handle); }
            set { Shape_SetLocalTransform(m_Handle, value); }
        } 

        public CBody m_Body { get { return new CBody(GetParent()); } }
        public CShape m_Sibling { get { return new CShape(CEntity.Entity_GetSibling(m_Handle)); } }
        
        public Bounds m_Bounds { get { return Shape_GetBounds(m_Handle); } }

        public float m_Emissive {
            get { return Shape_GetEmissiveScale(m_Handle); }
            set { Shape_SetEmissiveScale(m_Handle, value); }
        }

        public uint GetParent() { return CEntity.Entity_GetParent(m_Handle); }
        public uint GetSibling() { return CEntity.Entity_GetSibling(m_Handle); }
        public uint GetChild() { return CEntity.Entity_GetChild(m_Handle); }

        public void Destroy() { CEntity.Entity_Destroy(m_Handle); }
        public void SetParent(uint iParent) { CEntity.Entity_SetParent(m_Handle, iParent); }

        public bool m_Broken {  get { return Shape_IsBroken(m_Handle); } }

        public void SetCollisionFilter(sbyte CollisionLayer, sbyte CollisionMask) { Shape_SetCollisionFilter(m_Handle, CollisionLayer, CollisionMask); }

        public static bool LoadVox(uint iHandle, string sVoxPath, string sVoxName, float fScale)
        {
            if (!sVoxPath.EndsWith(".vox"))
                Log.Warning("LoadVox called with path that does not end in .vox");

            string? sVerifiedPath = CSledgeUtils.GetValidPath(sVoxPath, Assembly.GetCallingAssembly());
            if (sVerifiedPath == null)
                throw new Exception("invalid path passed to LoadVox");

            return Shape_LoadVox(iHandle, sVerifiedPath.Replace("\\", "/"), sVoxName, fScale);
        }

        public CShape(CBody Body) { m_Handle = Shape_Create(Body.m_Handle); }
        public CShape(uint iShapeHandle) { m_Handle = iShapeHandle; }
    }
}