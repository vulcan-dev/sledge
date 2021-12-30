using System.Runtime.InteropServices;
using System.Numerics;

namespace SledgeLib
{
    public class Player
    {
        [DllImport("sledge.dll")] internal static extern void SetPlayerHealth(float fValue);
        public static dSetFloat SetHealth = SetPlayerHealth;
        [DllImport("sledge.dll")] internal static extern float GetPlayerHealth();
        public static dGetFloat GetHealth = GetPlayerHealth;

        [DllImport("sledge.dll")] internal static extern void SetPlayerPosition(Vector3 vPosition);
        public static dSetVector3 SetPosition = SetPlayerPosition;

        [DllImport("sledge.dll")] internal static extern Vector3 GetPlayerPosition();
        public static dGetVector3 GetPosition = GetPlayerPosition;

        [DllImport("sledge.dll")] internal static extern void SetPlayerVelocity(Vector3 vVelocity);
        public static dSetVector3 SetVelocity = SetPlayerVelocity;
        [DllImport("sledge.dll")] internal static extern Vector3 GetPlayerVelocity();
        public static dGetVector3 GetVelocity = GetPlayerVelocity;

        [DllImport("sledge.dll")] public static extern void SetPlayerCameraTransform(Transform tCameraTransform);
        public static dSetTransform SetCameraTransform = SetPlayerCameraTransform;
        [DllImport("sledge.dll")] public static extern Transform GetPlayerCameraTransform();
        public static dGetTransform GetCameraTransform = GetPlayerCameraTransform;

        [DllImport("sledge.dll")] public static extern Vector2 GetPlayerMovementInput();
        public static dGetVector2 GetMovement = GetPlayerMovementInput;
        [DllImport("sledge.dll")] public static extern Vector2 GetPlayerMouseInput();
        public static dGetVector2 GetMouse = GetPlayerMouseInput;

        [DllImport("sledge.dll")] public static extern void Respawn();

        [DllImport("sledge.dll")] public static extern uint GetGrabbedBody();
        [DllImport("sledge.dll")] public static extern void ReleaseGrabbedBody();
    }
}