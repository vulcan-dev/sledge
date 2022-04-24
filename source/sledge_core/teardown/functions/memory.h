#pragma once

namespace Teardown
{
	inline void (*free)	(void* pMemoryBlock);
	inline void* (*malloc)	(size_t lSize);
	inline void* (*calloc)	(size_t lCount, size_t lSize);
	inline void* (*realloc) (void* pMemoryBlock, size_t lSize);
	inline void* (*memmove) (void* pDest, void* pSource, size_t lSize);
}