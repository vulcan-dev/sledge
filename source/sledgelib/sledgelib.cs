﻿//using System.Runtime.CompilerServices;

//public static class SSledgeInternal
//{
//    //public delegate void WriteLogDelegate(string cMsg);

//    [MethodImpl(MethodImplOptions.InternalCall)]
//    public static extern void WriteLog(string cMsg);
//}

//public class SledgeLib
//{
//    //public static SSledgeInternal Internal;
//}

using System.Runtime.InteropServices;

public delegate void WriteLogDelegate(string cMsg, params object[] args);
struct CSledgeInternal
{
    public WriteLogDelegate WriteLog;
    public WriteLogDelegate WriteError;
}

class SledgeLoader
{
    public delegate IntPtr GetInternalAPIDelegate();
    public delegate bool InitDelegate(GetInternalAPIDelegate GetInternalAPI);

    public static bool Init(GetInternalAPIDelegate GetInternalAPI)
    {
        IntPtr pInternal = GetInternalAPI();

        CSledgeInternal Internal = (CSledgeInternal)System.Runtime.InteropServices.Marshal.PtrToStructure(pInternal, typeof(CSledgeInternal));

        SledgeLib._SetInternal(Internal);

        Internal.WriteLog("sledgelib loaded");
        return true;
    }
}

class SledgeLib
{
    public static CSledgeInternal _Internal;

    public static void _SetInternal(CSledgeInternal Internal)
    {
        _Internal = Internal;
    }

    public static void WriteLog(string sLog)
    {
        _Internal.WriteLog(sLog);
    }
}