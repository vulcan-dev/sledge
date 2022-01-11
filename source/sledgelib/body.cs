using System.Runtime.InteropServices;
using System.Numerics;

namespace SledgeLib
{
    public class CBody
    {
        [DllImport("sledge.dll")] private static extern uint Body_Create();
        [DllImport("sledge.dll")] private static extern void Body_Destroy(uint iBodyHandle);

        [DllImport("sledge.dll")] private static extern Transform Body_GetTransform(uint iHandle);
        [DllImport("sledge.dll")] private static extern void Body_SetTransform(uint iHandle, Transform tTransform);

        [DllImport("sledge.dll")] private static extern Vector3 Body_GetPosition(uint iHandle);
        [DllImport("sledge.dll")] private static extern void Body_SetPosition(uint iHandle, Vector3 vVelocity);

        [DllImport("sledge.dll")] private static extern Quaternion Body_GetRotation(uint iHandle);
        [DllImport("sledge.dll")] private static extern void Body_SetRotation(uint iHandle, Quaternion vRotation);

        [DllImport("sledge.dll")] private static extern Vector3 Body_GetVelocity(uint iHandle);
        [DllImport("sledge.dll")] private static extern void Body_SetVelocity(uint iHandle, Vector3 vVelocity);

        [DllImport("sledge.dll")] private static extern Vector3 Body_GetAngularVelocity(uint iHandle);
        [DllImport("sledge.dll")] private static extern void Body_SetAngularVelocity(uint iHandle, Vector3 vVelocity);

        [DllImport("sledge.dll")] private static extern bool Body_GetDynamic(uint iHandle);
        [DllImport("sledge.dll")] private static extern void Body_SetDynamic(uint iHandle, bool bDynamic);

        [DllImport("sledge.dll")] private static extern bool Body_GetActive(uint iHandle);
        [DllImport("sledge.dll")] private static extern void Body_SetActive(uint iHandle, bool bDynamic);

        [DllImport("sledge.dll")] private static extern float GetBodyMass(uint iHandle);

        public readonly uint m_Handle;

        public Transform m_Transform
        {
            get { return Body_GetTransform(m_Handle); }
            set { Body_SetTransform(m_Handle, value); }
        }

        public Vector3 m_Position
        {
            get { return Body_GetPosition(m_Handle); }
            set { Body_SetPosition(m_Handle, value); }
        }

        public Quaternion m_Rotation
        {
            get { return Body_GetRotation(m_Handle); }
            set { Body_SetRotation(m_Handle, value); }

        }

        public Vector3 m_Velocity
        {
            get { return Body_GetVelocity(m_Handle); }
            set { Body_SetVelocity(m_Handle, value); }
        }

        public Vector3 m_AngularVelocity
        {
            get { return Body_GetAngularVelocity(m_Handle); }
            set { Body_SetAngularVelocity(m_Handle, value); }
        }

        public bool m_Active
        {
            get { return Body_GetActive(m_Handle); }
            set { Body_SetActive(m_Handle, value); }

        }

        public float m_Mass { get { return GetBodyMass(m_Handle); } }

        public CBody() { m_Handle = Body_Create(); }
        public CBody(uint iHandle) { m_Handle = iHandle; }

        ~CBody() { Body_Destroy(m_Handle); }

        public uint GetParent() { return CEntity.Entity_GetParent(m_Handle); }
        public uint GetSibling() { return CEntity.Entity_GetSibling(m_Handle); }
        public uint GetChild() { return CEntity.Entity_GetChild(m_Handle); }

        public List<CShape> GetChildren()
        {
            uint iLastHandle = GetChild();

            List<CShape> ChildrenList = new List<CShape>();
            while (iLastHandle != 0)
            {
                CShape Shape = new CShape(iLastHandle);
                ChildrenList.Add(Shape);
                iLastHandle = Shape.m_Sibling.m_Handle;
            }

            return ChildrenList;
        }
    }
}