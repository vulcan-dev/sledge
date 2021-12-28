using System.Runtime.InteropServices;

namespace SledgeLib
{
    public enum EGameState : uint
    {
        Splash = 1,
        Menu = 2,
        UI = 3,
        Playing = 4,
        Editing = 5,
        Quitting = 6
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
    }
}