
using System.Numerics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace SledgeLib
{
    public class CPlayer
    {
        /*
         * private functions
         */
        [DllImport("sledge.dll")] private static extern float Player_GetHealth();
        [DllImport("sledge.dll")] private static extern void Player_SetHealth(float fValue);

        [DllImport("sledge.dll")] private static extern Vector3 Player_GetPosition();
        [DllImport("sledge.dll")] private static extern void Player_SetPosition(Vector3 vValue);

        [DllImport("sledge.dll")] private static extern Vector3 Player_GetVelocity();
        [DllImport("sledge.dll")] private static extern void Player_SetVelocity(Vector3 vValue);

        [DllImport("sledge.dll")] private static extern Transform Player_GetCameraTransform();
        [DllImport("sledge.dll")] private static extern void Player_SetCameraTransform(Transform tValue);

        [DllImport("sledge.dll")] private static extern Vector2 Player_GetCameraAngles();
        [DllImport("sledge.dll")] private static extern void Player_SetCameraAngles(Vector2 vValue);

        [DllImport("sledge.dll")] private static extern Transform Player_GetToolTransform();
        [DllImport("sledge.dll")] private static extern void Player_SetToolTransform(Transform tValue);

        [DllImport("sledge.dll")] private static extern Vector2 Player_GetMovementInput();

        [DllImport("sledge.dll")] private static extern void Player_ReleaseGrab();
        [DllImport("sledge.dll")] private static extern uint Player_GetGrabbedShape();
        [DllImport("sledge.dll")] private static extern uint Player_GetGrabbedBody();

        [DllImport("sledge.dll")] private static extern uint Player_GetToolBody();
        [DllImport("sledge.dll")] private static extern uint Player_GetVehicleBody();

        [DllImport("sledge.dll")] private static extern int Player_GetLastToolIdx();

        [DllImport("sledge.dll")] private static extern void Player_RegisterTool(string sId, string sName, string sFile, uint iGroup = 6);

        [DllImport("sledge.dll")] private static extern void Player_Respawn();
        [DllImport("sledge.dll")] private static extern bool Player_M1Down();
        [DllImport("sledge.dll")] private static extern bool Player_M2Down();

        public static float m_Health
        {
            get { return Player_GetHealth(); }
            set { Player_SetHealth(value); }
        }

        public static Vector3 m_Position
        {
            get { return Player_GetPosition(); }
            set { Player_SetPosition(value); }
        }

        public static Vector3 m_Velocity
        {
            get { return Player_GetVelocity(); }
            set { Player_SetVelocity(value); }
        }

        public static Transform m_CameraTransform
        {
            get { return Player_GetCameraTransform(); }
            set { Player_SetCameraTransform(value); }
        }
        public static Vector2 m_CameraAngles
        {
            get { return Player_GetCameraAngles(); }
            set { Player_SetCameraAngles(value); }
        }
        public static Transform m_ToolTransform
        {
            get { return Player_GetToolTransform(); }
            set { Player_SetToolTransform(value); }
        }

        public static Vector2 m_MovementInput { get { return Player_GetMovementInput(); } }


        public static dVoid ReleaseGrab = Player_ReleaseGrab;
        public static CShape m_GrabbedShape { get { return new CShape(Player_GetGrabbedShape()); } }
        public static CBody m_GrabbedBody { get { return new CBody(Player_GetGrabbedBody()); } }
        public static CBody m_ToolBody { get { return new CBody(Player_GetToolBody());} }

        public static CBody m_VehicleBody { get { return new CBody(Player_GetVehicleBody()); } }

        public static void RegisterTool(string sId, string sName, string sFile, int iGroup = 6, bool bEnabled = false)
        {
            if (!Game.IsPlaying())
                throw new Exception("can't register tools while not playing");

            if (!sFile.EndsWith(".vox"))
                Log.Warning("LoadVox called with path that does not end in .vox");

            string? sVerifiedPath = CSledgeUtils.GetValidPath(sFile, Assembly.GetCallingAssembly());
            if (sVerifiedPath == null)
                throw new Exception("unable to resolve path");

            if (iGroup < 1 || iGroup > 6)
                iGroup = 6;

            int iIdx = Player_GetLastToolIdx() + 1;

            Player_RegisterTool(sId, sName, "RAW:" + sVerifiedPath.Replace("\\", "/"), (uint)iGroup);

            Registry.SetString("game.tool." + sId + ".name", sName);
            Registry.SetInt("game.tool." + sId + ".index", iIdx);
            Registry.SetInt("game.tool." + sId + ".group", iGroup);
            Registry.SetBool("game.tool." + sId + ".enabled", bEnabled);
        }

        public static bool m_M1Down { get { return Player_M1Down(); } }
        public static bool m_M2Down { get { return Player_M2Down(); } }

        public static dVoid Respawn = Player_Respawn;
    }
}