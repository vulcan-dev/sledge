using System.Runtime.InteropServices;
using System.Text;

namespace SledgeLib
{
    public class Tags
    {
        [DllImport("sledge.dll")] public static extern void SetTag(uint iHandle, string sName, string sValue);

        [DllImport("sledge.dll")] public static extern bool HasTag(uint iHandle, string sName);

        [DllImport("sledge.dll")] public static extern void RemoveTag(uint iHandle, string sName);

        [DllImport("sledge.dll")] public static extern void _GetTag(uint iHandle, string sTagName, StringBuilder sReturn, uint iReturnLen);
        public static string GetTag(uint iHandle, string sTagName)
        {
            StringBuilder sReturn = new StringBuilder(255);

            _GetTag(iHandle, sTagName, sReturn, 255);
            return sReturn.ToString();
        }
    }
}