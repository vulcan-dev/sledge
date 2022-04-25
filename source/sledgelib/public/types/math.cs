using System.Numerics;

namespace SledgeLib
{
    public struct Transform
    {
        public Vector3 Position;
        public Quaternion Rotation;

        public Transform()
        {
            Position = new Vector3();
            Rotation = new Quaternion(0, 0, 0, 1);
        }
        public Transform(Vector3 vPosition, Quaternion qRotation)
        {
            Position = vPosition; Rotation = qRotation;
        }
    }

    public struct Bounds
    {
        public Vector3 m_Min;
        public Vector3 m_Max;

        public Bounds()
        {
            m_Min = new Vector3();
            m_Max = new Vector3();
        }

        public Bounds(Vector3 vMin, Vector3 vMax)
        {
            m_Min = vMin;
            m_Max = vMax;
        }
    }
}