#include "teardown/hooks.h"
#include "teardown/offsets.h"
#include "teardown/classes/game.h"
#include "teardown/classes/renderer.h"

#include "sledge/vr.h"

#include "globals.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <gl/glew.h>

#include <windef.h>
#include <processthreadsapi.h>
#include <memoryapi.h>
#include <detours.h>

typedef void (*tDrawScene) (Renderer*, unsigned int, unsigned int, unsigned int, glm::mat4*, glm::mat4*);
tDrawScene _DrawScene;

/*
	TO-DO:
		either rewrite drawscene altogether or detour some of the functions unnecessary to vr (functions in charge of DOF, Motion Blur, Bloom, etc) and stop them from being called
*/

void hDrawScene(Renderer* pRenderer, unsigned int a2, unsigned int /*iWidth*/, unsigned int /*iHeight*/, glm::mat4* /*mProjection*/, glm::mat4* /*mView*/) {
	glm::mat4 PositionMatrix = glm::translate(glm::mat4(1.0f), -SledgeVR::vPlayerPos);

	/*
		FIXME:
			converting to mat3 then to mat4 is probably way too expensive to be done per frame
			this needs to be improved
	*/
	glm::mat4 mHMDRot = glm::mat4(glm::mat3(SledgeVR::mHMDPose));
	mHMDRot = glm::rotate(mHMDRot, SledgeVR::fPlayerRotation, glm::vec3(0, 1, 0));

	glm::mat4 LeftViewMatrix = mHMDRot * PositionMatrix;
	glm::mat4 RightViewMatrix = mHMDRot * PositionMatrix;

	/*
		TO-DO:
			There's ghosting between the two eyes (due to the rt?)
			it might be necessary to render them to two different buffers to fix it
	*/
	_DrawScene(pRenderer, a2, SledgeVR::iRTHeight, SledgeVR::iRTWidth, &SledgeVR::mProjectionLeftEye, &LeftViewMatrix);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_TEXTURE_2D);
	SledgeVR::DrawLeftEye();

	g_Renderer->m_StableVpMatrix = g_Renderer->m_OldStableVpMatrix;

	_DrawScene(pRenderer, a2, SledgeVR::iRTHeight, SledgeVR::iRTWidth, &SledgeVR::mProjectionRightEye, &RightViewMatrix);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_TEXTURE_2D);
	SledgeVR::DrawRightEye();
}

void Teardown::Hooks::DrawScene::Hook() {
	/*
		get function address
	*/
	_DrawScene = reinterpret_cast<tDrawScene>(g_BaseAddress + g_Offsets["DrawScene"]);
	LogVerbose("DrawScene: {0}", reinterpret_cast<void*>(_DrawScene));

	/*
		apply detour
	*/
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_DrawScene, hDrawScene);
	DetourTransactionCommit();
}

void Teardown::Hooks::DrawScene::Unhook() {
	/*
		undo the detour
	*/
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_DrawScene, hDrawScene);
	DetourTransactionCommit();
}