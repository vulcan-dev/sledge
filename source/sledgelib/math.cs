using System.Numerics;

public struct Transform
{
    public Vector3 Position;
    public Quaternion Rotation;

    public Transform()
    {
        Position = new Vector3();
        Rotation = new Quaternion();
    }
    public Transform(Vector3 vPosition, Quaternion qRotation)
    {
        Position = vPosition; Rotation = qRotation;
    }
}