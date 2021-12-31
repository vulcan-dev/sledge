using System.Runtime.InteropServices;
using System.Text;

namespace SledgeLib
{
    public class Registry
    {
        [DllImport("sledge.dll")] public static extern void SetBool(string sKeyName, bool bValue);
        [DllImport("sledge.dll")] public static extern bool GetBool(string sKeyName);

        [DllImport("sledge.dll")] public static extern void SetInt(string sKeyName, int iValue);
        [DllImport("sledge.dll")] public static extern int GetInt(string sKeyName);

        [DllImport("sledge.dll")] public static extern void SetFloat(string sKeyName, float fValue);
        [DllImport("sledge.dll")] public static extern float GetFloat(string sKeyName);

        [DllImport("sledge.dll")] public static extern void SetString(string sKeyName, string sValue);
        [DllImport("sledge.dll")] private static extern void _GetString(string sKeyName, StringBuilder sReturn, uint iReturnLen);
        public static string GetString(string sKeyName)
        {
            StringBuilder sReturn = new StringBuilder(255);

            _GetString(sKeyName, sReturn, 255);
            return sReturn.ToString();
        }
    }
}