#include "teardown/functions.h"
#include "teardown/signatures.h"
#include "teardown/functions/memory.h"

#include "util/memory.h"
#include "util/log.h"

struct SScanneable {
	CSignature m_Sig;
	void* m_Function;
	const char* m_Name;
};

static const SScanneable aFunctions[] = {
	// Memory
	{ Signatures::Free, &Teardown::free, "Teardown::Free" },
	{ Signatures::Alloc, &Teardown::alloc, "Teardown::Alloc" },
	{ Signatures::Realloc, &Teardown::realloc, "Teardown::Realloc" },
};

void Teardown::GetFunctionAddresses() {
	for (const SScanneable &Scanneable : aFunctions) {
		unsigned long long dwFuncAddr = Memory::dwFindPattern(Scanneable.m_Sig);
		memcpy(Scanneable.m_Function, &dwFuncAddr, sizeof(void*));
		LogVerbose("{} - {}", Scanneable.m_Name, reinterpret_cast<void*>(dwFuncAddr));
	}
}