#pragma once

namespace Teardown {
	inline void (*GenVoxTexture)(class Vox* pVox);
	inline void (*GenVoxBuffers)(class Vox* pVox);

	inline class Vox* (*LoadVox) (class small_string* ssPath, small_string* ssObject, float fScale);

	inline void (*SetBodyDynamic) (void* pPhysics, class Body* pBody);
	inline void (*InitializeBody) (class Body* pBody);

	inline void (*UpdateBodyActive) (void* pPhysics, class Body* pBody);
}