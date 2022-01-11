#include "teardown/functions/tags.h"
#include "teardown/classes/scene.h"
#include "teardown/utils.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void SetTag(unsigned int iHandle, char* cName, char* cValue) {
	CEntity* Entity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!Entity) return;

	small_string ssName(cName);
	small_string ssValue(cValue);
	Teardown::SetTag(Entity, &ssName, &ssValue);
}

sledgelib_func bool HasTag(unsigned int iHandle, char* cName) {
	CEntity* Entity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!Entity) return false;

	small_string ssName(cName);
	return Teardown::HasTag(Entity, &ssName);
}

sledgelib_func void RemoveTag(unsigned int iHandle, char* cName) {
	CEntity* Entity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!Entity) return;

	small_string ssName(cName);
	Teardown::RemoveTag(Entity, &ssName);
}

sledgelib_func void _GetTag(unsigned int iHandle, char* cName, char* cReturn, unsigned int iReturnBufferSize) {
	CEntity* Entity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!Entity) return;

	small_string ssReturn("null");
	small_string ssName(cName);

	Teardown::GetTag(Entity, &ssReturn, &ssName);

	if (ssReturn.len() > iReturnBufferSize) {
		memcpy(cReturn, ssReturn.c_str(), iReturnBufferSize);
		cReturn[iReturnBufferSize] = '\0';
	}
	else {
		memcpy(cReturn, ssReturn.c_str(), ssReturn.len());
		cReturn[ssReturn.len()] = '\0';
	}
}