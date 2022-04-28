#include "teardown/hooks.h"
#include "teardown/offsets.h"
#include "teardown/classes/renderer.h"
#include "teardown/classes/game.h"
#include "teardown/classes/player.h"

#include "sledge/vr.h"

#include "globals.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <gl/glew.h>

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef void (*tDrawScene) (Renderer*, unsigned int, unsigned int, unsigned int, glm::mat4*, glm::mat4*);
tDrawScene _DrawScene;

void hDrawScene(Renderer* pRenderer, unsigned int a2, unsigned int /*iWidth*/, unsigned int /*iHeight*/, glm::mat4* /*mProjection*/, glm::mat4* /*mView*/) {
	SledgeVR::Update();

	glm::mat4 PositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-g_Game->m_Player->m_Position.x, -(g_Game->m_Player->m_Position.y), -g_Game->m_Player->m_Position.z));
	
	glm::mat4 LeftViewMatrix = SledgeVR::mHMDPose * PositionMatrix;
	glm::mat4 RightViewMatrix = SledgeVR::mHMDPose * PositionMatrix;

	_DrawScene(pRenderer, a2, SledgeVR::iRTHeight, SledgeVR::iRTWidth, &SledgeVR::mProjectionLeftEye, &LeftViewMatrix);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_TEXTURE_2D);
	SledgeVR::DrawLeftEye();

	_DrawScene(pRenderer, a2, SledgeVR::iRTHeight, SledgeVR::iRTWidth, &SledgeVR::mProjectionRightEye, &RightViewMatrix);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_TEXTURE_2D);
	SledgeVR::DrawRightEye();
}

void Teardown::Hooks::DrawScene::Hook() {
	_DrawScene = reinterpret_cast<tDrawScene>(g_BaseAddress + g_Offsets["DrawScene"]);
	LogVerbose("DrawScene: {0}", reinterpret_cast<void*>(_DrawScene));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_DrawScene, hDrawScene);
	DetourTransactionCommit();
}

void Teardown::Hooks::DrawScene::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_DrawScene, hDrawScene);
	DetourTransactionCommit();
}