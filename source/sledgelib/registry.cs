using System.Runtime.InteropServices;
using System.Text;

namespace SledgeLib
{
    public class Registry
    {
        [DllImport("sledge_core.dll")] public static extern void SetBool(string KeyName, bool Value);
        [DllImport("sledge_core.dll")] public static extern bool GetBool(string KeyName);

        [DllImport("sledge_core.dll")] public static extern void SetInt(string KeyName, int Value);
        [DllImport("sledge_core.dll")] public static extern int GetInt(string KeyName);

        [DllImport("sledge_core.dll")] public static extern void SetFloat(string KeyName, float Value);
        [DllImport("sledge_core.dll")] public static extern float GetFloat(string KeyName);

        [DllImport("sledge_core.dll")] public static extern void SetString(string KeyName, string Value);
        [DllImport("sledge_core.dll")] private static extern void _GetString(string KeyName, StringBuilder Return, uint ReturnLen);
        public static string GetString(string KeyName, uint ExpectedMaxLen=100)
        {
            StringBuilder Return = new StringBuilder((int)ExpectedMaxLen);
            _GetString(KeyName, Return, ExpectedMaxLen);
            return Return.ToString();
        }
    }
}