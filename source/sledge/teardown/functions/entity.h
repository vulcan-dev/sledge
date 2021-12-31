#pragma once

#include "teardown/classes/entities.h"

namespace Teardown {
	namespace Entity {
		typedef void (*tVoxInitializer) (CVox* pVox);
		inline tVoxInitializer GenVoxTexture;
		inline tVoxInitializer GenVoxBuffers;

		typedef void (*tSetBodyDynamic) (CBody* pBody, bool bDynamic);
		inline tSetBodyDynamic SetBodyDynamic;

		typedef void (*tInitializeBody) (CBody* pBody);
		inline tInitializeBody InitializeBody;

		typedef CVox* (*tLoadVox) (class small_string* ssPath, small_string* ssObject, float fScale);
		inline tLoadVox LoadVox;
	}
}