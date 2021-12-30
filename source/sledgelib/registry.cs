﻿using System.Runtime.InteropServices;

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
    }
}