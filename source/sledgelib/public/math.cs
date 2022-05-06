using System;
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

    public class SLMath
    {
        /*
         * TO-DO:
         *      Organize (and optimize?) TransformToLocalPoint and TransformToParentPoint
         */
        public static Vector3 TransformToLocalPoint(Transform T, Vector3 V)
        {
            Quaternion R = T.Rotation;

            var v4 = R.Y;
            var v5 = V.Y;
            var v6 = R.Z;
            var v7 = R.W;
            var v8 = ((v5 * v4) + (V.X * R.X)) + (V.Z * v6);
            var v9 = v8 + v8;
            var v10 = ((v7 + v7) * v7) - 1.0;
            var v11 = (V.X * v6) - (V.Z * R.X);
            var v12 = ((V.Z * v10) - (((v5 * R.X) - (V.X * v4)) * (v7 + v7))) + (v9 * v6);

            Vector3 Out;
            Out.X = (float)((V.X * v10) - (((V.Z * v4) - (v5 * v6)) * (v7 + v7))) + (v9 * R.X);
            Out.Z = (float)v12;
            Out.Y = (float)((v5 * v10) - (v11 * (v7 + v7))) + (v9 * v4);
            return Out;
        }

        public static Vector3 TransformToParentPoint(Transform T, Vector3 V)
        {
            var v7 = T.Rotation.W + T.Rotation.W;
            var v8 = ((V.Y * T.Rotation.Y) + (V.X * T.Rotation.X)) + (V.Z * T.Rotation.Z);
            var v9 = v8 + v8;
            var v10 = (v7 * T.Rotation.W) - 1.0f;
            var v11 = (((V.Z * T.Rotation.Y) - (V.Y * T.Rotation.Z)) * v7) + (V.X * v10);
            var v12 = (V.X * T.Rotation.Z) - (V.Z * T.Rotation.X);
            var v14 = (((V.Y * T.Rotation.X) - (V.X * T.Rotation.Y)) * v7) + (V.Z * v10);

            Vector3 Out;
            Out.X = v11 + (v9 * T.Rotation.X);
            Out.Z = v14 + (v9 * T.Rotation.Z);
            Out.Y = ((v12 * v7) + (V.Y * v10)) + (v9 * T.Rotation.Y);
            return Out;
        }

        public static Transform GetShapeWorldTransform(Shape S)
        {
            Transform WorldTransform;
            Body B = new Body(S.GetParent());
            WorldTransform.Position = TransformToParentPoint(B.Transform, S.LocalTransform.Position) + B.Position;
            WorldTransform.Rotation = B.Rotation * S.LocalTransform.Rotation;
            return WorldTransform;
        }

        public static Quaternion QuatAxisAngle(Vector3 Rotation, float Angle)
        {
            float AngleRad = Angle * (float)(Math.PI / 180);

            Quaternion Q;
            var RadSin = Math.Sin(AngleRad / 2);
            Q.X = (float)(RadSin) * Rotation.X;
            Q.Y = (float)(RadSin) * Rotation.Y;
            Q.Z = (float)(RadSin) * Rotation.Z;
            Q.W = (float)(Math.Cos(AngleRad / 2));
            return Q;
        }
    }
}