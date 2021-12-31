namespace Teardown {
	namespace Entities {
		typedef void (*tVoxInitializer) (class CVox* pVox);
		tVoxInitializer GenVoxTexture;
		tVoxInitializer GenVoxBuffers;
	}
}