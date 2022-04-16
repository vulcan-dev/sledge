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

    /*
     * interfaces used to send functions to C++
     */
    internal delegate bool dBoolRet();
    internal delegate CallbackInvokersInterface dCallbackInterfaceGetter();
    
    [StructLayout(LayoutKind.Explicit)]
    internal struct UnmanagedInterface
    {
        [FieldOffset(0x0)]
        internal dBoolRet Init;

        [FieldOffset(0x8)]
        internal dBoolRet Shutdown;

        [FieldOffset(0x10)]
        internal dCallbackInterfaceGetter GetCallbackInterface;
    }

    [StructLayout(LayoutKind.Explicit)]
    internal struct CallbackInvokersInterface
    {
        [FieldOffset(0x0)]
        internal dStateChangeCallback StateChange;
        [FieldOffset(0x8)]
        internal dStringCallback LevelLoad;
        [FieldOffset(0x10)]
        internal dCallback PreUpdate;
        [FieldOffset(0x18)]
        internal dCallback PostUpdate;
        [FieldOffset(0x20)]
        internal dFloatCallback PrePlayerUpdate;
        [FieldOffset(0x28)]
        internal dFloatCallback PostPlayerUpdate;
    }
}