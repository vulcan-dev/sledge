#include "teardown/classes/entities.h"

namespace Teardown {
	namespace Entity {
		typedef void (*tVoxInitializer) (class CVox* pVox);
		tVoxInitializer GenVoxTexture;
		tVoxInitializer GenVoxBuffers;

		typedef CVox* (*tLoadVox) (class small_string* ssPath, small_string* ssObject, float fScale);
		tLoadVox LoadVox;
	}
}