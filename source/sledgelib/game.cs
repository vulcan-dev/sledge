using System.Runtime.InteropServices;
using System.Text;

namespace SledgeLib
{
    public class Game
    {
        [DllImport("sledge_core.dll")] private static extern void _GetLevelPath(StringBuilder ReturnString, uint ReturnLength);
        public static string m_LevelPath
        {
            get {
                StringBuilder ReturnString = new StringBuilder(255);
                _GetLevelPath(ReturnString, 255);
                return ReturnString.ToString();
            }
        }

        [DllImport("sledge_core.dll")] private static extern void _GetLevelLayers(StringBuilder ReturnString, uint ReturnLength);
        public static string m_LevelLayers
        {
            get {
                StringBuilder ReturnString = new StringBuilder(100);
                _GetLevelLayers(ReturnString, 100);
                return ReturnString.ToString();
            }
        }

        [DllImport("sledge_core.dll")] private static extern void _GetLevelId(StringBuilder ReturnString, uint ReturnLength);
        public static string m_LevelId
        {
            get {
                StringBuilder ReturnString = new StringBuilder(100);
                _GetLevelId(ReturnString, 100);
                return ReturnString.ToString();
            }
        }

        [DllImport("sledge_core.dll")] private static extern void _LoadLevel(string LevelPath, string LevelLayers, string LevelId, string LevelModId);
        public static void LoadLevel(string? LevelPath, string? LevelLayers, string? LevelId, string? LevelModId)
        {
            if (LevelPath == null)
                LevelPath = "";

            if (LevelId == null)
                LevelId = "";

            if (LevelLayers == null)
                LevelLayers = "";

            if (LevelModId == null)
                LevelModId = "";

            _LoadLevel(LevelPath, LevelLayers, LevelId, LevelModId);
        }
    }
}