#pragma once

namespace Teardown {
	typedef void* (*tmalloc) (unsigned long long);
	typedef void (*tfree) (void*);

	inline tfree free;
	inline tmalloc alloc;
}