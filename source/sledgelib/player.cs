using System.Runtime.InteropServices;
using System.Numerics;
using System.Reflection;

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

        [DllImport("sledge.dll")] public static extern Vector2 GetPlayerCameraAngles();
        public static dGetVector2 GetCameraAngles = GetPlayerCameraAngles;
        [DllImport("sledge.dll")] public static extern void SetPlayerCameraAngles(Vector2 vAngle);
        public static dSetVector2 SetCameraAngles = SetPlayerCameraAngles;

        [DllImport("sledge.dll")] public static extern void Respawn();

        [DllImport("sledge.dll")] public static extern uint GetGrabbedShape();
        [DllImport("sledge.dll")] public static extern uint GetGrabbedBody();
        [DllImport("sledge.dll")] public static extern void ReleaseGrab();

        [DllImport("sledge.dll")] private static extern void _RegisterTool(string sId, string sName, string sFile, uint iGroup = 6);
        [DllImport("sledge.dll")] private static extern int _GetLastToolIdx();
        public static void RegisterTool(string sId, string sName, string sFile, int iGroup = 6, bool bEnabled = false)
        {
            if (!Game.IsPlaying())
            {
                Log.Warning("Attempted to register tool {0} while not in-game", sName);
                return;
            }

            if (!sFile.EndsWith(".vox"))
                Log.Warning("RegisterTool called with file name that does not end in .vox");

            if (!File.Exists(sFile))
            {
                Assembly Caller = Assembly.GetCallingAssembly(); ;
                string? sModPath = ModLoader.GetPathByAssembly(Caller);

                if (sModPath == null) return;
                if (!File.Exists(sModPath + "/" + sFile))
                {
                    Log.General("unable to find vox at: {0}", sModPath + "/" + sFile);
                    return;
                }
                sFile = "RAW:" + sModPath + "/" + sFile;
            }

            sFile = sFile.Replace("\\", "/");

            if (iGroup < 1 || iGroup > 6)
                iGroup = 6;

            int iIdx = _GetLastToolIdx() + 1;

            _RegisterTool(sId, sName, sFile, (uint)iGroup);
            
            Registry.SetString("game.tool." + sId + ".name", sName);
            Registry.SetInt("game.tool." + sId + ".index", iIdx);
            Registry.SetInt("game.tool." + sId + ".group", iGroup);
            Registry.SetBool("game.tool." + sId + ".enabled", bEnabled);
        }

        [DllImport("sledge.dll")] private static extern uint GetPlayerVehicleBody();
        public static dGetUInt GetVehicleBody = GetPlayerVehicleBody;

        [DllImport("sledge.dll")] public static extern bool M1Down();
        [DllImport("sledge.dll")] public static extern bool M2Down();

        [DllImport("sledge.dll")] public static extern uint GetToolBody();
        [DllImport("sledge.dll")] public static extern void SetToolTransform(Transform tTransform, float fToolSway);
    }
}