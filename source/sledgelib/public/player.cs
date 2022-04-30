using System;
using System.IO;
using System.Numerics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace SledgeLib
{
    public class Player
    {
        [DllImport("sledge_core.dll")] private static extern float Player_GetHealth();
        [DllImport("sledge_core.dll")] private static extern void Player_SetHealth(float Value);
        [DllImport("sledge_core.dll")] private static extern Vector3 Player_GetPosition();
        [DllImport("sledge_core.dll")] private static extern void Player_SetPosition(Vector3 Value);
        [DllImport("sledge_core.dll")] private static extern Vector3 Player_GetVelocity();
        [DllImport("sledge_core.dll")] private static extern void Player_SetVelocity(Vector3 Value);
        [DllImport("sledge_core.dll")] private static extern Transform Player_GetCameraTransform();
        //[DllImport("sledge_core.dll")] private static extern void Player_SetCameraTransform(Transform tValue);
        [DllImport("sledge_core.dll")] private static extern Vector3 Player_GetCameraPosition();
        [DllImport("sledge_core.dll")] private static extern void Player_SetCameraPosition(Vector3 Value);
        [DllImport("sledge_core.dll")] private static extern Vector2 Player_GetCameraAngles();
        [DllImport("sledge_core.dll")] private static extern void Player_SetCameraAngles(Vector2 Value);
        [DllImport("sledge_core.dll")] private static extern Transform Player_GetToolTransform();
        [DllImport("sledge_core.dll")] private static extern void Player_SetToolTransform(Transform Value);
        [DllImport("sledge_core.dll")] private static extern Vector2 Player_GetMovementKeys();
        [DllImport("sledge_core.dll")] private static extern void Player_ReleaseGrab();
        [DllImport("sledge_core.dll")] private static extern uint Player_GetGrabbedShape();
        [DllImport("sledge_core.dll")] private static extern uint Player_GetGrabbedBody();
        [DllImport("sledge_core.dll")] private static extern uint Player_GetToolBody();
        [DllImport("sledge_core.dll")] private static extern uint Player_GetVehicleBody();
        [DllImport("sledge_core.dll")] private static extern int Player_GetLastToolIdx();
        [DllImport("sledge_core.dll")] private static extern void Player_RegisterTool(string Id, string Name, string File, uint Group = 6);
        [DllImport("sledge_core.dll")] private static extern void Player_Respawn();
        [DllImport("sledge_core.dll")] private static extern bool Player_M1Down();
        [DllImport("sledge_core.dll")] private static extern bool Player_M2Down();
        [DllImport("sledge_core.dll")] internal static extern Transform Player_GetSpawnTransform();

        public static float Health
        {
            get { return Player_GetHealth(); }
            set { Player_SetHealth(value); }
        }
        public static Vector3 Position
        {
            get { return Player_GetPosition(); }
            set { Player_SetPosition(value); }
        }
        public static Vector3 Velocity
        {
            get { return Player_GetVelocity(); }
            set { Player_SetVelocity(value); }
        }
        public static Transform CameraTransform
        {
            get { return Player_GetCameraTransform(); }
        }
        public static Vector3 CameraPosition
        {
            get { return Player_GetCameraPosition(); }
            set { Player_SetCameraPosition(value); }
        }
        public static Vector2 CameraAngles
        {
            get { return Player_GetCameraAngles(); }
            set { Player_SetCameraAngles(value); }
        }
        public static Transform ToolTransform
        {
            get { return Player_GetToolTransform(); }
            set { Player_SetToolTransform(value); }
        }
        public static Transform SpawnTransform
        {
            get { return Player_GetSpawnTransform(); }
        }
        public static Vector2 MovementInput { get { return Player_GetMovementKeys(); } }

        public static dVoid ReleaseGrab = Player_ReleaseGrab;
        public static Shape GrabbedShape { get { return new Shape(Player_GetGrabbedShape()); } }
        public static Body GrabbedBody { get { return new Body(Player_GetGrabbedBody()); } }
        public static Body ToolBody { get { return new Body(Player_GetToolBody()); } }
        public static Body VehicleBody { get { return new Body(Player_GetVehicleBody()); } }
        public static bool M1Down { get { return Player_M1Down(); } }
        public static bool M2Down { get { return Player_M2Down(); } }

        public static dVoid Respawn = Player_Respawn;
        public static void RegisterTool(string Id, string Name, string Model, uint Group = 6, bool Enabled = false)
        {
            if (!Game.Playing)
                throw new Exception("can't register tools while not playing");

            if (!Model.EndsWith(".vox"))
                throw new Exception("RegisterTool called with path that does not end in .vox");

            string? sModFolder = ModManager.GetModFolderFromAssembly(Assembly.GetCallingAssembly());
            if (sModFolder == null)
                throw new System.Exception("RegisterTool called from mod that does not have a data folder");

            string sResolvedVoxPath = string.Format("{0}\\{1}", sModFolder, Model);
            if (!File.Exists(sResolvedVoxPath))
                throw new System.Exception(string.Format("File {0} does not exist", sResolvedVoxPath));

            if (Group < 1 || Group > 6)
                Group = 6;

            int iIdx = Player_GetLastToolIdx() + 1;

            Player_RegisterTool(Id, Name, "RAW:" + sResolvedVoxPath.Replace("\\", "/"), Group);

            Registry.SetString("game.tool." + Id + ".name", Name);
            Registry.SetInt("game.tool." + Id + ".index", iIdx);
            Registry.SetInt("game.tool." + Id + ".group", (int)Group);
            Registry.SetBool("game.tool." + Id + ".enabled", Enabled);
        }
    }
}