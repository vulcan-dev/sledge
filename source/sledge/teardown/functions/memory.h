#pragma once

typedef void  (*tfree)		(void* pMemoryBlock);
typedef void* (*tmalloc)	(size_t lSize);
typedef void* (*tcalloc)	(size_t lCount, size_t lSize);
typedef void* (*trealloc)	(void* pMemoryBlock, size_t lSize);
typedef void* (*tmemmove)	(void* pDest, void* pSource, size_t lSize);

namespace Teardown 
{
	inline tfree free = nullptr;
	inline tmalloc alloc = nullptr;
	inline tcalloc calloc = nullptr;
	inline trealloc realloc = nullptr;
	inline tmemmove memmove = nullptr;
}