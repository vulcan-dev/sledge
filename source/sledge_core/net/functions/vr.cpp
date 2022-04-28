#include "sledge/vr.h"
#include "teardown/types/math.h"
#include "globals.h"

#define sledgelib_func extern "C" __declspec(dllexport)

enum EAnalogType : unsigned int
{
    Analog = 0,
    Grip = 1,
    Trigger = 2
};

/*
    to-do: optimize or align C# struct with glm::mat4
*/
sledgelib_func Matrix4x4 _GetHMDPose() {
    Matrix4x4 mPose;
    memcpy(&mPose, &SledgeVR::mHMDPose, sizeof(Matrix4x4));
    return mPose;
}

sledgelib_func Matrix4x4 _GetPose(unsigned int iHand) {
    Matrix4x4 mPose;
    memcpy(&mPose, &SledgeVR::aControllers[iHand].m_Pose, sizeof(Matrix4x4));
    return mPose;
}

sledgelib_func float _GetPlayerScale() { return SledgeVR::vPlayerScale.x; }
sledgelib_func void _SetPlayerScale(float fScale) { SledgeVR::vPlayerScale = glm::vec3(fScale); }

sledgelib_func vr::InputAnalogActionData_t _GetAnalogData(unsigned int iHand, EAnalogType Type) {
    switch (Type) {
    case Analog:
        return SledgeVR::aControllers[iHand].m_Analog;
    case Grip:
        return SledgeVR::aControllers[iHand].m_Grip;
    case Trigger:
        return SledgeVR::aControllers[iHand].m_Trigger;
    }
    return vr::InputAnalogActionData_t();
}

sledgelib_func bool _GetTopButtonDown(unsigned int iHand) { return SledgeVR::aControllers[iHand].m_TopButton; }
sledgelib_func bool _GetBottomButtonDown(unsigned int iHand) { return SledgeVR::aControllers[iHand].m_BottomButton; }

sledgelib_func bool _GetVREnabled() { return g_VR; }

sledgelib_func float _GetVRPlayerRotation() { return SledgeVR::fPlayerRotation; };
sledgelib_func void _SetVRPlayerRotation(float fRotation) { SledgeVR::fPlayerRotation = fRotation; };
