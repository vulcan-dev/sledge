using System.Runtime.InteropServices;
using System.Numerics;
using System;

namespace SledgeLib
{
    public enum EVRHand : uint
    {
        Left = 0,
        Right = 1
    }
    public enum EAnalogType : uint
    {
        Analog = 0,
        Grip = 1,
        Trigger = 2
    }

    public struct InputAnalogData
    {
        public bool Active;
        private IntPtr Padding;
        public Vector3 State;
        public Vector3 Delta;
        public float UpdateTime;
    }

    public class VR
    {
        [DllImport("sledge_core.dll")] internal static extern Matrix4x4 _GetHMDPose();
        [DllImport("sledge_core.dll")] internal static extern InputAnalogData _GetAnalogData(EVRHand Hand, EAnalogType Type);
        [DllImport("sledge_core.dll")] internal static extern Matrix4x4 _GetControllerPose(EVRHand Hand);
        [DllImport("sledge_core.dll")] internal static extern bool _GetTopButtonDown(EVRHand Hand);
        [DllImport("sledge_core.dll")] internal static extern bool _GetBottomButtonDown(EVRHand Hand);
        [DllImport("sledge_core.dll")] internal static extern bool _GetVREnabled();
        [DllImport("sledge_core.dll")] internal static extern Vector3 _GetVRPlayerPosition();
        [DllImport("sledge_core.dll")] internal static extern void _SetVRPlayerPosition(Vector3 Value);
        [DllImport("sledge_core.dll")] internal static extern Vector3 _GetVRWorldScale();
        [DllImport("sledge_core.dll")] internal static extern void _SetVRWorldScale(Vector3 Scale);
        [DllImport("sledge_core.dll")] internal static extern float _GetVRPlayerRotation();
        [DllImport("sledge_core.dll")] internal static extern void _SetVRPlayerRotation(float Value);

        public static bool Enabled { get { return _GetVREnabled(); } }

        public static Vector3 Scale
        {
            get { return _GetVRWorldScale(); }
            set { _SetVRWorldScale(value); }
        }

        public static Vector3 Position
        {
            get { return _GetVRPlayerPosition(); }
            set { _SetVRPlayerPosition(value); }
        }

        public static float Rotation
        {
            get { return _GetVRPlayerRotation(); }
            set { _SetVRPlayerRotation(value); }
        }

        public static bool LeftTopButtonDown{ get { return _GetTopButtonDown(EVRHand.Left); } }
        public static bool RightTopButtonDown { get { return _GetTopButtonDown(EVRHand.Right); } }

        public static bool LeftBottomButtonDown { get { return _GetBottomButtonDown(EVRHand.Left); } }
        public static bool RightBottomButtonDown { get { return _GetBottomButtonDown(EVRHand.Right); } }

        public static Matrix4x4 HMDPose {  get { return _GetHMDPose(); } }
        public static Matrix4x4 LeftControllerPose { get { return _GetControllerPose(EVRHand.Left); } }
        public static Matrix4x4 RightControllerPose { get { return _GetControllerPose(EVRHand.Right); } }

        public static InputAnalogData LeftTrigger { get { return _GetAnalogData(EVRHand.Left, EAnalogType.Trigger); } }
        public static InputAnalogData RightTrigger { get { return _GetAnalogData(EVRHand.Right, EAnalogType.Trigger); } }
        public static InputAnalogData LeftGrip { get { return _GetAnalogData(EVRHand.Left, EAnalogType.Grip); } }
        public static InputAnalogData RightGrip { get { return _GetAnalogData(EVRHand.Right, EAnalogType.Grip); } }
        public static InputAnalogData LeftAnalog { get { return _GetAnalogData(EVRHand.Left, EAnalogType.Analog); } }
        public static InputAnalogData RightAnalog { get { return _GetAnalogData(EVRHand.Right, EAnalogType.Analog); } }
    }
}