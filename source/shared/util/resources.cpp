#include "resources.h"

#include <windef.h>
#include <WinBase.h>
#include <spng.h>

#include <GL/glew.h>

unsigned int Resources::TextureFromPNG(const char* cResourceName) {
	HRSRC hResourceInfo = FindResourceA(NULL, cResourceName, "RC_DATA");
	if (!hResourceInfo)
		return 0;

	HGLOBAL hResourceGlob = LoadResource(NULL, hResourceInfo);
	if (!hResourceGlob)
		return 0;

	DWORD dwResourceSize = SizeofResource(0, hResourceInfo);

	LPVOID ResourceData = LockResource(hResourceGlob);

	spng_ctx* PNGCtx = spng_ctx_new(0);

	spng_set_png_buffer(PNGCtx, ResourceData, dwResourceSize);

	spng_ihdr IHDR{};
	if (spng_get_ihdr(PNGCtx, &IHDR))
		return 0;

	size_t lOutSize;
	if (spng_decoded_image_size(PNGCtx, SPNG_FMT_RGBA8, &lOutSize))
		return 0;

	uint8_t* Buffer = new uint8_t[lOutSize];
	if (spng_decode_image(PNGCtx, Buffer, lOutSize, SPNG_FMT_RGBA8, 0))
		return 0;

	GLuint Texture;
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IHDR.width, IHDR.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Buffer);

	delete[] Buffer;
	spng_ctx_free(PNGCtx);

	return Texture;
}