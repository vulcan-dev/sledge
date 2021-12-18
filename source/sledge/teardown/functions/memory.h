#pragma once

namespace Teardown {
	typedef void* (*tmalloc) (unsigned long long);
	typedef void (*tfree) (void*);
	typedef void (*trealloc) (void*, unsigned __int64);

	inline tfree free;
	inline tmalloc alloc;
	inline trealloc realloc;
}