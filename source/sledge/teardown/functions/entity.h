#pragma once

#include "teardown/classes/entities.h"

namespace Teardown {
	namespace Entity {
		typedef void (*tVoxInitializer) (CVox* pVox);
		inline tVoxInitializer GenVoxTexture;
		inline tVoxInitializer GenVoxBuffers;

		typedef void (*tUpdateBodyDynamic) (void* pPhysics, CBody* pBody);
		inline tUpdateBodyDynamic UpdateBodyDynamic;

		typedef void (*tInitializeBody) (CBody* pBody);
		inline tInitializeBody InitializeBody;

		typedef void (*tUnknownBodyFunction) (void* pPhysics, CBody* pBody, bool bActive);
		inline tUnknownBodyFunction UnknownBodyFunction;

		typedef CVox* (*tLoadVox) (class small_string* ssPath, small_string* ssObject, float fScale);
		inline tLoadVox LoadVox;
	}
}