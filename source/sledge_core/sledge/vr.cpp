#include "sledge/vr.h"
#include "teardown/classes/renderer.h"
#include "globals.h"

#include <fmt/format.h>

#include "util/log.h"

#include <glm/gtx/transform.hpp>

#include <GL/glew.h>
#include <openvr.h>


/*
	interface pointers
*/
vr::IVRSystem* System;
vr::IVRCompositor* Compositor;

/*
	poses
*/
vr::TrackedDevicePose_t TrackedDevicePoses[vr::k_unMaxTrackedDeviceCount];
glm::mat4 TrackedDevicePosesMatrices[vr::k_unMaxTrackedDeviceCount];

/*
	input
*/
vr::VRActionSetHandle_t MainActionSet = vr::k_ulInvalidActionSetHandle;

bool SledgeVR::Init() {
	LogInfo("Initializing OpenVR");

	if (!vr::VR_IsRuntimeInstalled()) {
		LogError("OpenVR Runtime not installed");
		return false;
	}

	if (!vr::VR_IsHmdPresent()) {
		LogError("No VR headset detected (Is SteamVR running?)");
		return false;
	}

	vr::EVRInitError Err;
	System = vr::VR_Init(&Err, vr::VRApplication_Scene);
	if (Err != vr::HmdError::VRInitError_None) {
		LogError("VR_Init error: {}", vr::VR_GetVRInitErrorAsEnglishDescription(Err));
		return false;
	}
	LogInfo("OpenVR Inititialized");

	Compositor = vr::VRCompositor();
	if (!Compositor) {
		LogError("VRCompositor returned nil");
		return false;
	}
	LogVerbose("VRCompositor initialized");

	System->GetRecommendedRenderTargetSize(&SledgeVR::iRTWidth, &SledgeVR::iRTHeight);
	LogInfo("Rendering at: {}x{}", SledgeVR::iRTWidth, SledgeVR::iRTHeight);

	/*
		get matrices
	*/
	vr::HmdMatrix34_t mEyeToHeadTransform = System->GetEyeToHeadTransform(vr::Hmd_Eye::Eye_Left);
	mViewLeftEye = glm::mat4(	mEyeToHeadTransform.m[0][0], mEyeToHeadTransform.m[1][0], mEyeToHeadTransform.m[2][0], 0.0f,
								mEyeToHeadTransform.m[0][1], mEyeToHeadTransform.m[1][1], mEyeToHeadTransform.m[2][1], 0.0f,
								mEyeToHeadTransform.m[0][2], mEyeToHeadTransform.m[1][2], mEyeToHeadTransform.m[2][2], 0.0f,
								mEyeToHeadTransform.m[0][3], mEyeToHeadTransform.m[1][3], mEyeToHeadTransform.m[2][3], 1.0f);

	mEyeToHeadTransform = System->GetEyeToHeadTransform(vr::Hmd_Eye::Eye_Right);
	mViewRightEye = glm::mat4(	mEyeToHeadTransform.m[0][0], mEyeToHeadTransform.m[1][0], mEyeToHeadTransform.m[2][0], 0.0f,
								mEyeToHeadTransform.m[0][1], mEyeToHeadTransform.m[1][1], mEyeToHeadTransform.m[2][1], 0.0f,
								mEyeToHeadTransform.m[0][2], mEyeToHeadTransform.m[1][2], mEyeToHeadTransform.m[2][2], 0.0f,
								mEyeToHeadTransform.m[0][3], mEyeToHeadTransform.m[1][3], mEyeToHeadTransform.m[2][3], 1.0f);

	vr::HmdMatrix44_t mEyeProj = System->GetProjectionMatrix(vr::Eye_Left, 0.1f, 500.f);
	mProjectionLeftEye = glm::mat4(	mEyeProj.m[0][0], mEyeProj.m[1][0], mEyeProj.m[2][0], mEyeProj.m[3][0],
									mEyeProj.m[0][1], mEyeProj.m[1][1], mEyeProj.m[2][1], mEyeProj.m[3][1],
									mEyeProj.m[0][2], mEyeProj.m[1][2], mEyeProj.m[2][2], mEyeProj.m[3][2],
									mEyeProj.m[0][3], mEyeProj.m[1][3], mEyeProj.m[2][3], mEyeProj.m[3][3]);

	mEyeProj = System->GetProjectionMatrix(vr::Eye_Right, 0.1f, 500.f);
	mProjectionRightEye = glm::mat4(mEyeProj.m[0][0], mEyeProj.m[1][0], mEyeProj.m[2][0], mEyeProj.m[3][0],
									mEyeProj.m[0][1], mEyeProj.m[1][1], mEyeProj.m[2][1], mEyeProj.m[3][1],
									mEyeProj.m[0][2], mEyeProj.m[1][2], mEyeProj.m[2][2], mEyeProj.m[3][2],
									mEyeProj.m[0][3], mEyeProj.m[1][3], mEyeProj.m[2][3], mEyeProj.m[3][3]);

	std::string sActionManifestPath = fmt::format("{}\\sledge_actionmanifest.json", g_ModulePath);

	LogVerbose("Loading action manifest from: {}", sActionManifestPath);

	vr::EVRInputError InputErr = vr::VRInput()->SetActionManifestPath(sActionManifestPath.c_str());
	if (InputErr != vr::EVRInputError::VRInputError_None) {
		LogError("SetActionManifestPath error: {}", vr::VR_GetVRInitErrorAsEnglishDescription(Err));
		return false;
	}

	InputErr =vr::VRInput()->GetInputSourceHandle("/user/hand/left", &aControllers[LEFT_HAND].m_Source);
	InputErr =vr::VRInput()->GetInputSourceHandle("/user/hand/right", &aControllers[RIGHT_HAND].m_Source);

	InputErr =vr::VRInput()->GetActionSetHandle("/actions/main", &MainActionSet);

	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/LeftHand", &aControllers[LEFT_HAND].m_PoseAction);
	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/RightHand", &aControllers[RIGHT_HAND].m_PoseAction);

	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/GrabL", &aControllers[LEFT_HAND].m_GripAction);
	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/GrabR", &aControllers[RIGHT_HAND].m_GripAction);

	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/TriggerL", &aControllers[LEFT_HAND].m_TriggerAction);
	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/TriggerR", &aControllers[RIGHT_HAND].m_TriggerAction);
	
	
	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/LeftAnalog", &aControllers[LEFT_HAND].m_AnalogAction);
	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/RightAnalog", &aControllers[RIGHT_HAND].m_AnalogAction);
	
	
	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/LeftAnalogClick", &aControllers[LEFT_HAND].m_AnalogClickAction);
	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/RightAnalogClick", &aControllers[RIGHT_HAND].m_AnalogClickAction);

	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/LeftTopButton", &aControllers[LEFT_HAND].m_TopButtonAction);
	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/RightTopButton", &aControllers[RIGHT_HAND].m_TopButtonAction);

	InputErr = vr::VRInput()->GetActionHandle("/actions/main/in/LeftBottomButton", &aControllers[LEFT_HAND].m_BottomButtonAction);
	InputErr = vr::VRInput()->GetActionHandle("/actions/main/in/RightBottomButton", &aControllers[RIGHT_HAND].m_BottomButtonAction);

	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/VibrationL", &aControllers[LEFT_HAND].m_VibrationAction);
	InputErr =vr::VRInput()->GetActionHandle("/actions/main/in/VibrationR", &aControllers[RIGHT_HAND].m_VibrationAction);

	return true;
}

void ProcessVREvent(vr::VREvent_t /*VREvent*/) {
	//switch (VREvent.eventType) {
	//case vr::VREvent_TrackedDeviceActivated:
	//	vr::ETrackedDeviceClass DevClass = System->GetTrackedDeviceClass(VREvent.trackedDeviceIndex);
	//	if (DevClass == vr::ETrackedDeviceClass::TrackedDeviceClass_Controller) {
	//		vr::ETrackedControllerRole Role = System->GetControllerRoleForTrackedDeviceIndex(VREvent.trackedDeviceIndex);

	//		if (Role == vr::TrackedControllerRole_RightHand)
	//			iRightControllerId = VREvent.trackedDeviceIndex;

	//		if (Role == vr::TrackedControllerRole_LeftHand)
	//			iLeftControllerId = VREvent.trackedDeviceIndex;
	//	}
	//	break;
	//case vr::VREvent_TrackedDeviceDeactivated:
	//	if (VREvent.trackedDeviceIndex == iRightControllerId)
	//		iRightControllerId = 0;
	//	else if (VREvent.trackedDeviceIndex == iLeftControllerId)
	//		iLeftControllerId = 0;
	//	break;
	//}
}

void PollControllerInput(int iHand) {
	SledgeVR::ControllerInfo* Controller = &SledgeVR::aControllers[iHand];

	vr::InputDigitalActionData_t ActionData;

	vr::VRInput()->GetDigitalActionData(Controller->m_BottomButtonAction, &ActionData, sizeof(ActionData), vr::k_ulInvalidInputValueHandle);
	Controller->m_BottomButton= ActionData.bActive && ActionData.bState;
	vr::VRInput()->GetDigitalActionData(Controller->m_TopButtonAction, &ActionData, sizeof(ActionData), vr::k_ulInvalidInputValueHandle);
	Controller->m_TopButton= ActionData.bActive && ActionData.bState;

	vr::VRInput()->GetAnalogActionData(Controller->m_AnalogAction, &Controller->m_Analog, sizeof(vr::InputAnalogActionData_t), vr::k_ulInvalidInputValueHandle);
	vr::VRInput()->GetAnalogActionData(Controller->m_GripAction, &Controller->m_Grip, sizeof(vr::InputAnalogActionData_t), vr::k_ulInvalidInputValueHandle);
	vr::VRInput()->GetAnalogActionData(Controller->m_TriggerAction, &Controller->m_Trigger, sizeof(vr::InputAnalogActionData_t), vr::k_ulInvalidInputValueHandle);

	vr::InputPoseActionData_t PoseData;
	vr::VRInput()->GetPoseActionDataForNextFrame(Controller->m_PoseAction, vr::TrackingUniverseStanding, &PoseData, sizeof(PoseData), vr::k_ulInvalidInputValueHandle);

	if (!PoseData.bActive || !PoseData.pose.bPoseIsValid)
		Controller->m_Active = false;
	else {
		Controller->m_Active = true;

		vr::HmdMatrix34_t Pose = PoseData.pose.mDeviceToAbsoluteTracking;
		Controller->m_Pose= glm::mat4(Pose.m[0][0], Pose.m[1][0], Pose.m[2][0], 0.f,
			Pose.m[0][1], Pose.m[1][1], Pose.m[2][1], 0.f,
			Pose.m[0][2], Pose.m[1][2], Pose.m[2][2], 0.f,
			Pose.m[0][3], Pose.m[1][3], Pose.m[2][3], 1.f);
	}
}

void SledgeVR::Update() {
	//if (System) {
		//vr::VREvent_t VREvent;
		//while (System->PollNextEvent(&VREvent, sizeof(VREvent)))
		//	ProcessVREvent(VREvent);

	Compositor->WaitGetPoses(TrackedDevicePoses, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	if (TrackedDevicePoses[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
		vr::HmdMatrix34_t Pose = TrackedDevicePoses[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking;
		mHMDPose = glm::mat4(	Pose.m[0][0], Pose.m[1][0], Pose.m[2][0], 0.f,
								Pose.m[0][1], Pose.m[1][1], Pose.m[2][1], 0.f,
								Pose.m[0][2], Pose.m[1][2], Pose.m[2][2], 0.f,
								Pose.m[0][3], Pose.m[1][3], Pose.m[2][3], 1.f);
		mHMDPose = glm::inverse(mHMDPose);
		mHMDPose = glm::scale(mHMDPose, vWorldScale);
		mHMDPose = glm::rotate(mHMDPose, fPlayerRotation, glm::vec3(0, 1, 0));
	}

	vr::VRActiveActionSet_t ActiveActionSet = { 0 };
	ActiveActionSet.ulActionSet = MainActionSet;
	vr::VRInput()->UpdateActionState(&ActiveActionSet, sizeof(ActiveActionSet), 1); // TO-DO: SLOW FUNCTION - Move to another thread

	PollControllerInput(LEFT_HAND);
	PollControllerInput(RIGHT_HAND);
}

void SledgeVR::DrawLeftEye() {
	glBindTexture(GL_TEXTURE_2D, g_Renderer->m_FinalBuffer.m_FramebufferTextureId);
	vr::Texture_t EyeTexture = {
		.handle = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Renderer->m_FinalBuffer.m_FramebufferTextureId)),
		.eType = vr::TextureType_OpenGL,
		.eColorSpace = vr::ColorSpace_Auto
	};
	Compositor->Submit(vr::Eye_Left, &EyeTexture);
}

void SledgeVR::DrawRightEye() {
	glBindTexture(GL_TEXTURE_2D, g_Renderer->m_FinalBuffer.m_FramebufferTextureId);
	vr::Texture_t EyeTexture = {
		.handle = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Renderer->m_FinalBuffer.m_FramebufferTextureId)),
		.eType = vr::TextureType_OpenGL,
		.eColorSpace = vr::ColorSpace_Auto
	};
	Compositor->Submit(vr::Eye_Right, &EyeTexture);
}

void SledgeVR::Shutdown() {
	vr::VR_Shutdown();
}