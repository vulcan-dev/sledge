#pragma once

#include "teardown/types/small_string.h"
#include "teardown/types/rendertypes.h"
#include "teardown/types/math.h"

class Renderer
{
public:
	class UnknownRenderRelatedClass *m_UnknownRenderRelatedClass; //0x0000
	char pad_0008[8]; //0x0008
	class Scene *m_Scene; //0x0010
	char pad_0018[8]; //0x0018
	class DOFSettings *m_DOFSettings; //0x0020
	char pad_0028[16]; //0x0028
	bool m_DOFOverriden; //0x0038
	char pad_0039[1843]; //0x0039
	unsigned int m_WindowResX; //0x076C
	unsigned int m_WindowResY; //0x0770
	unsigned int m_ResX; //0x0774
	unsigned int m_ResY; //0x0778
	unsigned int N0000068A; //0x077C
	unsigned int N0000156C; //0x0780
	float m_ZNear; //0x0784 default: 0.200
	float m_ZFar; //0x0788 default 499.968
	unsigned int m_RenderedFrames; //0x078C
	Matrix4x4 m_PerspectiveProjectionMatrix; //0x0790
	char pad_07D0[76]; //0x07D0
	Matrix4x4 m_VpMatrix; //0x081C
	Matrix4x4 m_VpInvMatrix; //0x085C
	char pad_089C[1852]; //0x089C
	ShaderInfo m_Sprites; //0x0FD8
	ShaderInfo m_Barrel; //0x1240
	ShaderInfo m_GBuffer; //0x14A8
	ShaderInfo m_GBufferRope; //0x1710
	ShaderInfo m_GBufferSky; //0x1978
	ShaderInfo m_gBufferWater; //0x1BE0
	ShaderInfo m_gBufferVox; //0x1E48
	ShaderInfo m_gBufferVoxClip; //0x20B0
	ShaderInfo m_gBufferTerminal; //0x2318
	ShaderInfo N00001733; //0x2580
	ShaderInfo m_gUnderwater; //0x27E8
	ShaderInfo m_UnderwaterSurface; //0x2A50
	ShaderInfo m_UnderwaterPost; //0x2CB8
	ShaderInfo m_AmbientLight; //0x2F20
	ShaderInfo m_DiffuseLight; //0x3188
	ShaderInfo m_DirectionalLight; //0x33F0
	ShaderInfo m_CCTV; //0x3658
	ShaderInfo m_Diffuse; //0x38C0
	ShaderInfo m_Denoise; //0x3B28
	ShaderInfo m_DenoiseRef; //0x3D90
	ShaderInfo m_Composite; //0x3FF8
	ShaderInfo m_Post; //0x4260
	ShaderInfo m_Reflection; //0x44C8
	char pad_4730[1848]; //0x4730
	ShaderInfo m_SmoothNormal; //0x4E68
	ShaderInfo m_Particles; //0x50D0
	ShaderInfo m_Fog; //0x5338
	ShaderInfo m_FogDirectional; //0x55A0
	ShaderInfo m_DenoiseFog; //0x5808
	ShaderInfo m_Wet; //0x5A70
	ShaderInfo m_Brightness; //0x5CD8
	ShaderInfo m_Exposure; //0x5F40
	ShaderInfo m_Character; //0x61A8
	ShaderInfo m_DebugVolume; //0x6410
	ShaderInfo m_SpritesDepthTest; //0x6678
	ShaderInfo m_Boundary; //0x68E0
	ShaderInfo m_MotionBlur; //0x6B48
	ShaderInfo m_OutlineVox; //0x6DB0
	ShaderInfo m_Outline; //0x7018
	ShaderInfo N000008A7; //0x7280
	ShaderInfo N000008A8; //0x74E8
	ShaderInfo m_VerticalHudBlur; //0x7750
	ShaderInfo m_HorizontalHudBlur; //0x79B8
	char pad_7C20[608]; //0x7C20
	FramebufferInfo m_ColorBuffer; //0x7E80
	FramebufferInfo m_NormalBuffer; //0x7EBC
	FramebufferInfo m_UnkBuffer; //0x7EF8
	FramebufferInfo m_UnkBuffer2; //0x7F34
	FramebufferInfo m_BloomBuffer; //0x7F70
	FramebufferInfo m_LightBuffer1; //0x7FAC
	FramebufferInfo m_LightBuffer2; //0x7FE8
	FramebufferInfo m_ReflectionBuffer; //0x8024
	FramebufferInfo m_FinalBuffer; //0x8060
	FramebufferInfo m_LightBuffer3; //0x809C
	FramebufferInfo m_ReflectionBuffer2; //0x80D8
	FramebufferInfo m_UnkBuffer3; //0x8114
	FramebufferInfo m_FogBuffer; //0x8150
	FramebufferInfo m_FogBuffer2; //0x818C
	FramebufferInfo m_DepthBuffer; //0x81C8
	FramebufferInfo m_LightBuffer4; //0x8204
	FramebufferInfo m_UnkBuffer4; //0x8240
	FramebufferInfo m_UnkBuffer5; //0x827C
	FramebufferInfo m_DepthBuffer2; //0x82B8
	FramebufferInfo m_UnkBuffer6; //0x82F4
	FramebufferInfo m_ReflectionBuffer3; //0x8330
	FramebufferInfo m_NormalBuffer2; //0x836C
	FramebufferInfo m_UnusedFramebuffer_Maybe; //0x83A8
	char pad_83E4[612]; //0x83E4
	Matrix4x4 m_StableVpMatrix; //0x8648
	Matrix4x4 m_UnstableVpMatrix; //0x8688
	char pad_86C8[8]; //0x86C8
	small_string m_RenderBuffer; //0x86D0
	bool m_DrawBounds; //0x86E0
	bool m_DrawBodies; //0x86E1
	bool m_DrawShapes; //0x86E2
	bool N0000157D; //0x86E3
	bool m_DrawShadowVolumes; //0x86E4
	bool m_HideParticles; //0x86E5
	char pad_86E6[4754]; //0x86E6
}; //Size: 0x9978

inline Renderer* g_Renderer;