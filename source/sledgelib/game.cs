using System.Runtime.InteropServices;

namespace SledgeLib
{
    public enum EGameState : uint
    {
        Unknown,
        Splash,
        Menu,
        UI,
        Playing,
        Editing,
        Quitting
    }

    public class Game
    {
        [DllImport("sledge.dll")] private static extern uint _GetState();
        [DllImport("sledge.dll")] private static extern void _SetState(uint eState);
        // hacky way to make these work with enums
        public static EGameState GetState() { return (EGameState)_GetState(); }
        public static void SetState(EGameState eState) { _SetState((uint)eState); }


        [DllImport("sledge.dll")] public static extern bool IsPlaying();

        [DllImport("sledge.dll")] public static extern bool GetMapOpen();
        [DllImport("sledge.dll")] public static extern void SetMapOpen(bool bOpen);

        [DllImport("sledge.dll")] public static extern void QuickSave();
        [DllImport("sledge.dll")] public static extern void QuickLoad();
    }
}