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

sledgelib_func Matrix4x4 _GetHMDPose() {
    Matrix4x4 mPose;
    memcpy(&mPose, &SledgeVR::mHMDPose, sizeof(Matrix4x4));
    return mPose;
}

sledgelib_func Matrix4x4 _GetControllerPose(unsigned int iHand) {
    Matrix4x4 mPose;
    memcpy(&mPose, &SledgeVR::aControllers[iHand].m_Pose, sizeof(Matrix4x4));
    return mPose;
}

sledgelib_func Vector3 _GetVRPlayerPosition() { return Vector3(SledgeVR::vPlayerPos.x, SledgeVR::vPlayerPos.y, SledgeVR::vPlayerPos.z); }
sledgelib_func void _SetVRPlayerPosition(glm::vec3 Pos) { SledgeVR::vPlayerPos = Pos; }

sledgelib_func Vector3 _GetVRWorldScale() { return Vector3(SledgeVR::vWorldScale.x, SledgeVR::vWorldScale.y, SledgeVR::vWorldScale.z);; }
sledgelib_func void _SetVRWorldScale(Vector3 vScale) { SledgeVR::vWorldScale = glm::vec3(vScale.x, vScale.y, vScale.z); }

sledgelib_func float _GetVRPlayerRotation() { return SledgeVR::fPlayerRotation; };
sledgelib_func void _SetVRPlayerRotation(float fRotation) { SledgeVR::fPlayerRotation = fRotation; };

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