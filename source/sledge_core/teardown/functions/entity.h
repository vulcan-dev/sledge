#pragma once
class Body;
class small_string;

namespace Teardown {
	inline void (*GenVoxTexture)(void* pVox);
	inline void (*GenVoxBuffers)(void* pVox);

	inline void* (*LoadVox) (small_string* ssPath, small_string* ssObject, float fScale);

	inline void (*InitializeBody) (Body* pBody);
	inline void (*UpdateBodyActive) (void* pPhysics, Body* pBody);
}