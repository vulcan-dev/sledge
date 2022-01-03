using System.Runtime.InteropServices;
using System.Numerics;
using System.Reflection;

namespace SledgeLib
{
    public class Shape
    {
        [DllImport("sledge.dll")] private static extern uint CreateShape(uint iBodyHandle);
        public delegate uint dCreateShape(uint iBodyHandle);
        public static dCreateShape Create = CreateShape;

        [DllImport("sledge.dll")] private static extern void DestroyShape(uint iShapeHandle);
        public delegate void dDestroyShape(uint iShapeHandle);
        public static dDestroyShape Destroy = DestroyShape;

        [DllImport("sledge.dll")] private static extern Transform GetShapeLocalTransform(uint iHandle);
        public static dGetTransformEntity GetLocalTransform = GetShapeLocalTransform;
        [DllImport("sledge.dll")] private static extern void SetShapeLocalTransform(uint iHandle, Transform tTransform);
        public static dSetTransformEntity SetLocalTransform = SetShapeLocalTransform;

        [DllImport("sledge.dll")] private static extern Transform GetShapeWorldTransform(uint iHandle);
        public static dGetTransformEntity GetWorldTransform = GetShapeWorldTransform;

        [DllImport("sledge.dll")] private static extern uint GetShapeBody(uint iHandle);
        public static dGetUIntEntity GetBody = GetShapeBody;

        [DllImport("sledge.dll")] private static extern uint GetShapeSibling(uint iHandle);
        public static dGetUIntEntity GetSibling = GetShapeSibling;

        [DllImport("sledge.dll")] private static extern Bounds GetShapeBounds(uint iHandle);
        public static dGetBoundsEntity GetBounds = GetShapeBounds;

        [DllImport("sledge.dll")] private static extern float GetShapeEmissiveScale(uint iHandle);
        public static dGetFloatEntity GetEmissiveScale = GetShapeEmissiveScale;
        [DllImport("sledge.dll")] private static extern void SetShapeEmissiveScale(uint iHandle, float fValue);
        public static dSetFloatEntity SetEmissiveScale = SetShapeEmissiveScale;

        [DllImport("sledge.dll")] private static extern bool GetShapeBroken(uint iHandle);
        public static dGetBoolEntity IsBroken = GetShapeBroken;

        [DllImport("sledge.dll")] private static extern void SetShapeCollisionFilter(uint iHandle, sbyte CollisionLayer, sbyte CollisionMask);
        public delegate void dSetShapeCollisionFilter(uint iHandle, sbyte CollisionLayer, sbyte CollisionMask);
        public static dSetShapeCollisionFilter SetCollisionFilter = SetShapeCollisionFilter;

        [DllImport("sledge.dll")] private static extern bool _LoadVox(uint iHandle, string sVoxPath, string sVoxName, float fScale);
        public static bool LoadVox(uint iHandle, string sVoxPath, string sVoxName, float fScale)
        {
            if (iHandle == 0)
            {
                Log.Error("Invalid handle passed to LoadVox");
                return false;
            }

            if (!sVoxPath.EndsWith(".vox"))
                Log.Warning("LoadVox called with path that does not end in .vox");

            if (!Path.IsPathRooted(sVoxPath))
            {
                if (File.Exists(Path.GetFullPath(sVoxPath)))
                {
                    sVoxPath = Path.GetFullPath(sVoxPath);
                } else
                {
                    Assembly Caller = Assembly.GetCallingAssembly(); ;
                    string? sModPath = ModLoader.GetPathByAssembly(Caller);

                    if (sModPath == null)
                    {
                        Log.Error("Assembly ModPath is null");
                        return false;
                    }

                    if (!File.Exists(sModPath + "/" + sVoxPath))
                    {
                        Log.Error("Unable to find vox file: {0}", sModPath + "/" + sVoxPath);
                        return false;
                    }

                    sVoxPath = sModPath + "/" + sVoxPath;
                }
            } else if (!File.Exists(sVoxPath))
            {
                Log.Error("Unable to find vox file: {0}", sVoxPath);
                return false;
            }

            if (!_LoadVox(iHandle, sVoxPath.Replace("\\", "/"), sVoxName, fScale))
            {
                Log.Error("Failed to load vox: {0}", sVoxPath);
                return false;
            }
            return true;
        }

    }
}