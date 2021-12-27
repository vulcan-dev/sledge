using System.Runtime.InteropServices;
using System.Numerics;

namespace SledgeLib
{
    public class Player
    {
        [DllImport("sledge.dll")] public static extern void SetHealth(float fValue);
        [DllImport("sledge.dll")] public static extern float GetHealth();

        [DllImport("sledge.dll")] public static extern void SetPosition(Vector3 vPosition);
        [DllImport("sledge.dll")] public static extern Vector3 GetPosition();

        [DllImport("sledge.dll")] public static extern void SetVelocity(Vector3 vVelocity);
        [DllImport("sledge.dll")] public static extern Vector3 GetVelocity();

        [DllImport("sledge.dll")] public static extern void SetCameraTransform(Transform tCameraTransform);
        [DllImport("sledge.dll")] public static extern Transform GetCameraTransform();

        [DllImport("sledge.dll")] public static extern Vector2 GetMovementInput();
        [DllImport("sledge.dll")] public static extern Vector2 GetMouseInput();

        [DllImport("sledge.dll")] public static extern void Respawn();
    }
}