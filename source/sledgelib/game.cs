using System.Runtime.InteropServices;

namespace SledgeLib
{
    public class Game
    {
        [DllImport("sledge.dll")] public static extern uint GetState();
        [DllImport("sledge.dll")] public static extern void SetState(uint iState);
        [DllImport("sledge.dll")] public static extern bool IsPlaying();

        [DllImport("sledge.dll")] public static extern bool GetMapOpen();
        [DllImport("sledge.dll")] public static extern void SetMapOpen(bool bOpen);
    }
}