using System.Runtime.InteropServices;
using System.Numerics;

namespace SledgeLib
{
    public class Body
    {
        [DllImport("sledge.dll")] private static extern float GetBodyMass(uint iHandle);
        public static dGetFloatEntity GetMass = GetBodyMass;

        [DllImport("sledge.dll")] private static extern bool GetBodyDynamic(uint iHandle);
        public static dGetBoolEntity IsDynamic = GetBodyDynamic;
        [DllImport("sledge.dll")] private static extern void SetBodyDynamic(uint iHandle, bool bDynamic);
        public static dSetBoolEntity SetDynamic = SetBodyDynamic;

        [DllImport("sledge.dll")] private static extern void SetBodyVelocity(uint iHandle, Vector3 vVelocity);
        public static dSetVector3Entity SetVelocity = SetBodyVelocity;
        [DllImport("sledge.dll")] private static extern Vector3 GetBodyVelocity(uint iHandle);
        public static dGetVector3Entity GetVelocity = GetBodyVelocity;

        [DllImport("sledge.dll")] private static extern Vector3 GetBodyAngularVelocity(uint iHandle);
        public static dGetVector3Entity GetAngularVelocity = GetBodyAngularVelocity;
        [DllImport("sledge.dll")] private static extern void SetBodyAngularVelocity(uint iHandle, Vector3 vVelocity);
        public static dSetVector3Entity SetAngularVelocity = SetBodyAngularVelocity;


        [DllImport("sledge.dll")] private static extern Vector3 GetBodyPosition(uint iHandle);
        public static dGetVector3Entity GetPosition = GetBodyPosition;
        [DllImport("sledge.dll")] private static extern void SetBodyPosition(uint iHandle, Vector3 vVelocity);
        public static dSetVector3Entity SetPosition = SetBodyPosition;

        [DllImport("sledge.dll")] private static extern Quaternion GetBodyRotation(uint iHandle);
        public static dGetQuaternionEntity GetRotation = GetBodyRotation;

        [DllImport("sledge.dll")] private static extern void SetBodyRotation(uint iHandle, Quaternion vRotation);
        public static dSetQuaternionEntity SetRotation = SetBodyRotation;
    }
}