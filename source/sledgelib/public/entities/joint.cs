using System.Numerics;
using System.Runtime.InteropServices;

namespace SledgeLib
{
    public enum EJointType
    {
        Ball = 1,
        Hinge = 2,
        Prismatic = 3
    }

    public class Joint {
        [DllImport("sledge_core.dll")] private static extern uint Joint_Create();
        [DllImport("sledge_core.dll")] private static extern void Joint_Attach(uint Joint, uint Parent, uint Child, Vector3 ChildJointPos, Vector3 ParentJointPos);
        [DllImport("sledge_core.dll")] private static extern void Joint_Detach(uint Joint, uint Child);
        [DllImport("sledge_core.dll")] private static extern bool Joint_GetCollide(uint Joint);
        [DllImport("sledge_core.dll")] private static extern void Joint_SetCollide(uint Joint, bool Value);
        [DllImport("sledge_core.dll")] private static extern bool Joint_GetSound(uint Joint);
        [DllImport("sledge_core.dll")] private static extern void Joint_SetSound(uint Joint, bool Value);
        [DllImport("sledge_core.dll")] private static extern EJointType Joint_GetType(uint Joint);
        [DllImport("sledge_core.dll")] private static extern void Joint_SetType(uint Joint, EJointType Value);
        [DllImport("sledge_core.dll")] private static extern float Joint_GetRotStrength(uint Joint);
        [DllImport("sledge_core.dll")] private static extern void Joint_SetRotStrength(uint Joint, float Value);
        [DllImport("sledge_core.dll")] private static extern float Joint_GetRotSpring(uint Joint);
        [DllImport("sledge_core.dll")] private static extern void Joint_SetRotSpring(uint Joint, float Value);
        [DllImport("sledge_core.dll")] private static extern float Joint_GetSize(uint Joint);
        [DllImport("sledge_core.dll")] private static extern void Joint_SetSize(uint Joint, float Value);

        public readonly uint Handle;

        public Joint() { Handle = Joint_Create(); }
        public Joint(uint Handle) { this.Handle = Handle; }

        public bool Collide
        {
            get { return Joint_GetCollide(Handle); }
            set { Joint_SetCollide(Handle, value); }
        }
        public EJointType Type
        {
            get { return Joint_GetType(Handle); }
            set { Joint_SetType(Handle, value); }
        }

        public float Size
        {
            get { return Joint_GetSize(Handle); }
            set { Joint_SetSize(Handle, value); }
        }

        public float RotStrength
        {
            get { return Joint_GetRotStrength(Handle); }
            set { Joint_SetRotStrength(Handle, value); }
        }

        public float RotSpring
        {
            get { return Joint_GetRotSpring(Handle); }
            set { Joint_SetRotSpring(Handle, value); }
        }

        public void Attach(Shape Parent, Shape Child, Vector3 ChildJointPos, Vector3 ParentJointPos)
        {
            Joint_Attach(this.Handle, Child.Handle, Parent.Handle, ChildJointPos, ParentJointPos);
        }

        public void Detach(Shape Parent)
        {
            Joint_Detach(this.Handle, Parent.Handle);
        }
    }
}