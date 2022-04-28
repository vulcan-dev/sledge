#pragma once

#include <glm/glm.hpp>
#include <openvr.h>

#define LEFT_HAND 0
#define RIGHT_HAND 1

/*
	TO-DO: Create FBOs for both eyes, draw scene there instead
*/
namespace SledgeVR {
	struct ControllerInfo {
		vr::VRInputValueHandle_t m_Source = vr::k_ulInvalidInputValueHandle;

		vr::VRActionHandle_t m_PoseAction = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_TriggerAction = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_GripAction = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_AnalogAction = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_AnalogClickAction = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_TopButtonAction = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_BottomButtonAction = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_VibrationAction = vr::k_ulInvalidInputValueHandle;

		vr::InputAnalogActionData_t m_Trigger;
		vr::InputAnalogActionData_t m_Grip;
		vr::InputAnalogActionData_t m_Analog;
		glm::mat4 m_Pose;

		bool m_TopButton = false;
		bool m_BottomButton = false;

		bool m_Active = false;
	};

	inline ControllerInfo aControllers[2];

	bool Init();
	void Shutdown();

	/*bool InitRender();*/
	void Update();

	void DrawLeftEye();
	void DrawRightEye();

	inline glm::mat4 mViewLeftEye;
	inline glm::mat4 mViewRightEye;

	inline glm::mat4 mProjectionLeftEye;
	inline glm::mat4 mProjectionRightEye;

	inline glm::mat4 mHMDPose;

	inline glm::vec3 vPlayerScale(0.8f);
	inline float fPlayerRotation = 0.f;

	inline unsigned int iRTWidth, iRTHeight;
}