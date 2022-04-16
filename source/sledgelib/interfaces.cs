using System.Runtime.InteropServices;

namespace SledgeLib
{
    /*
     *  class used by mods to declare their information and load / unload methods
     */
    public interface ISledgeMod
    {
        public string GetName();
        public string GetDescription();
        public string GetVersion();
        public string GetAuthor();

        public void Load();
        public void Unload();
    }


    public delegate bool dBoolRet();
    
    [StructLayout(LayoutKind.Explicit)]
    internal struct UnmanagedInterface
    {
        [FieldOffset(0)]
        internal dBoolRet Init;

        [FieldOffset(8)]
        internal dBoolRet Shutdown;
    }
}