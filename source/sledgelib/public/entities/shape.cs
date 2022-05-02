using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace SledgeLib
{
    public class Shape
    {
        [DllImport("sledge_core.dll")] private static extern uint Shape_Create(uint iBodyHandle);
        [DllImport("sledge_core.dll")] private static extern void Shape_Destroy(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern Transform Shape_GetLocalTransform(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern void Shape_SetLocalTransform(uint iHandle, Transform tTransform);
        [DllImport("sledge_core.dll")] private static extern float Shape_GetEmissive(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern void Shape_SetEmissive(uint iHandle, float fValue);
        [DllImport("sledge_core.dll")] private static extern Bounds Shape_GetBounds(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern bool Shape_GetBroken(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern void Shape_SetCollisionFilter(uint iHandle, sbyte CollisionLayer, sbyte CollisionMask);
        [DllImport("sledge_core.dll")] private static extern bool Shape_LoadVox(uint iHandle, string sVoxPath, string sVoxName, float fScale);

        public readonly uint Handle = 0;
        public uint GetParent() { return Entity.Entity_GetParent(Handle); }
        public uint GetSibling() { return Entity.Entity_GetSibling(Handle); }
        public uint GetChild() { return Entity.Entity_GetChild(Handle); }
        public void Destroy() { Entity.Entity_Destroy(Handle); }
        public void SetParent(uint iParent) { Entity.Entity_SetParent(Handle, iParent); }
        public Shape(Body Body) { Handle = Shape_Create(Body.Handle); }
        public Shape(uint iShapeHandle) { Handle = iShapeHandle; }

        public bool Broken { get { return Shape_GetBroken(Handle); } }

        public Bounds Bounds { get { return Shape_GetBounds(Handle); } }
        public float Emissive
        {
            get { return Shape_GetEmissive(Handle); }
            set { Shape_SetEmissive(Handle, value); }
        }

        public Transform LocalTransform
        {
            get { return Shape_GetLocalTransform(Handle); }
            set { Shape_SetLocalTransform(Handle, value); }
        }

        public static bool LoadVox(uint Handle, string VoxPath, string VoxName, float Scale)
        {
            if (!VoxPath.EndsWith(".vox"))
                throw new System.Exception("LoadVox called with path that does not end in .vox");

            string? ModFolder = ModManager.GetModFolderFromAssembly(Assembly.GetCallingAssembly());
            if (ModFolder == null)
                throw new System.Exception("LoadVox called from mod that does not have a data folder");

            string sResolvedVoxPath = string.Format("{0}\\{1}", ModFolder, VoxPath);
            if (!File.Exists(sResolvedVoxPath))
                throw new System.Exception(string.Format("File {0} does not exist", sResolvedVoxPath));

            return Shape_LoadVox(Handle, sResolvedVoxPath.Replace("\\", "/"), VoxName, Scale);
        }

        public static bool LoadVox(Shape TargetShape, string VoxPath, string VoxName, float Scale)
        {
            if (!VoxPath.EndsWith(".vox"))
                throw new System.Exception("LoadVox called with path that does not end in .vox");

            string? ModFolder = ModManager.GetModFolderFromAssembly(Assembly.GetCallingAssembly());
            if (ModFolder == null)
                throw new System.Exception("LoadVox called from mod that does not have a data folder");

            string sResolvedVoxPath = string.Format("{0}\\{1}", ModFolder, VoxPath);
            if (!File.Exists(sResolvedVoxPath))
                throw new System.Exception(string.Format("File {0} does not exist", sResolvedVoxPath));

            return Shape_LoadVox(TargetShape.Handle, sResolvedVoxPath.Replace("\\", "/"), VoxName, Scale);
        }
    }
}