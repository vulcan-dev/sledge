using System.Collections.Generic;
using System.Numerics;
using System.Runtime.InteropServices;

/*
 * TO-DO:   Fix Body.Active and Body.Dynamic setters
 *          (inactive / non dynamic physbuffers don't get generated)
 */

namespace SledgeLib
{
    public class Body
    {
        [DllImport("sledge_core.dll")] private static extern uint Body_Create();
        [DllImport("sledge_core.dll")] private static extern void Body_Destroy(uint iBodyHandle);
        [DllImport("sledge_core.dll")] private static extern Transform Body_GetTransform(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern void Body_SetTransform(uint iHandle, Transform tTransform);
        [DllImport("sledge_core.dll")] private static extern Vector3 Body_GetPosition(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern void Body_SetPosition(uint iHandle, Vector3 vVelocity);
        [DllImport("sledge_core.dll")] private static extern Quaternion Body_GetRotation(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern void Body_SetRotation(uint iHandle, Quaternion vRotation);
        [DllImport("sledge_core.dll")] private static extern Vector3 Body_GetVelocity(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern void Body_SetVelocity(uint iHandle, Vector3 vVelocity);
        [DllImport("sledge_core.dll")] private static extern Vector3 Body_GetAngularVelocity(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern void Body_SetAngularVelocity(uint iHandle, Vector3 vVelocity);
        [DllImport("sledge_core.dll")] private static extern bool Body_GetDynamic(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern void Body_SetDynamic(uint iHandle, bool bDynamic);
        [DllImport("sledge_core.dll")] private static extern bool Body_GetActive(uint iHandle);
        [DllImport("sledge_core.dll")] private static extern void Body_SetActive(uint iHandle, bool bDynamic);
        [DllImport("sledge_core.dll")] private static extern float Body_GetMass(uint iHandle);

        public readonly uint Handle;
        public uint GetParent() { return Entity.Entity_GetParent(Handle); }
        public uint GetSibling() { return Entity.Entity_GetSibling(Handle); }
        public uint GetChild() { return Entity.Entity_GetChild(Handle); }
        public void Destroy() { Entity.Entity_Destroy(Handle); }
        public void SetParent(uint iParent) { Entity.Entity_SetParent(Handle, iParent); }

        public Body() { Handle = Body_Create(); }
        public Body(uint iHandle) { Handle = iHandle; }

        public Transform Transform
        {
            get { return Body_GetTransform(Handle); }
            set { Body_SetTransform(Handle, value); }
        }

        public Vector3 Position
        {
            get { return Body_GetPosition(Handle); }
            set { Body_SetPosition(Handle, value); }
        }

        public Quaternion Rotation
        {
            get { return Body_GetRotation(Handle); }
            set { Body_SetRotation(Handle, value); }

        }

        public Vector3 Velocity
        {
            get { return Body_GetVelocity(Handle); }
            set { Body_SetVelocity(Handle, value); }
        }

        public Vector3 AngularVelocity
        {
            get { return Body_GetAngularVelocity(Handle); }
            set { Body_SetAngularVelocity(Handle, value); }
        }

        public bool Active
        {
            get { return Body_GetActive(Handle); }
            //set { Body_SetActive(Handle, value); } // currently broken

        }

        public bool Dynamic
        {
            get { return Body_GetDynamic(Handle); }
            //set { Body_SetDynamic(Handle, value); } // currently broken
        }

        public float Mass { get { return Body_GetMass(Handle); } }

        public List<Shape> GetChildren()
        {
            uint iSiblingHandle = GetChild();
            List<Shape> ChildrenList = new List<Shape>();
            while (iSiblingHandle != 0)
            {
                Shape Shape = new Shape(iSiblingHandle);
                ChildrenList.Add(Shape);
                iSiblingHandle = Shape.GetSibling();
            }
            return ChildrenList;
        }
    }
}