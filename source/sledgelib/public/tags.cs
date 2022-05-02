using System.Runtime.InteropServices;
using System.Text;

namespace SledgeLib
{
    public static class Tag
    {
        [DllImport("sledge_core.dll")] internal static extern void Tag_Set(uint Handle, string Name, string Value);
        [DllImport("sledge_core.dll")] internal static extern bool Tag_Has(uint Handle, string Name);
        [DllImport("sledge_core.dll")] internal static extern void Tag_Remove(uint Handle, string Name);
        [DllImport("sledge_core.dll")] internal static extern StringBuilder Tag_Get(uint Handle, string Name);

        public delegate void dSetTag(uint Handle, string Name, string Value);
        public static dSetTag Set = Tag_Set;

        public delegate bool dHasTag(uint Handle, string Name);
        public static dHasTag Has = Tag_Has;

        public delegate void dRemoveTag(uint Handle, string Name);
        public static dRemoveTag Remove = Tag_Remove;

        public static void Get(uint Handle, string Name)
        {
            Tag_Get(Handle, Name).ToString();
        }
    }
}