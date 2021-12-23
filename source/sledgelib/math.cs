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
}