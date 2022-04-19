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
        [DllImport("sledge_core.dll")] private static extern StringBuilder _GetString(string KeyName);
        public static string GetString(string KeyName) { return _GetString(KeyName).ToString(); }
    }
}