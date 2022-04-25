#include "teardown/functions/tags.h"
#include "teardown/classes/scene.h"
#include "teardown/utils.h"

#include "net/sledgelib.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void Tag_Set(unsigned int iHandle, char* cName, char* cValue) {
	Entity* pEntity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!pEntity) return;

	small_string* ssName = new small_string(cName);
	small_string* ssValue = new small_string(cValue);
	Teardown::SetTag(pEntity, ssName, ssValue);
	delete ssName;
	delete ssValue;
}

sledgelib_func void Tag_Remove(unsigned int iHandle, char* cName) {
	Entity* pEntity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!pEntity) return;

	small_string* ssName = new small_string(cName);
	Teardown::RemoveTag(pEntity, ssName);
	delete ssName;
}

sledgelib_func bool Tag_Has(unsigned int iHandle, char* cName) {
	Entity* pEntity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!pEntity) return false;

	small_string* ssName = new small_string(cName);
	bool bHasTag = Teardown::HasTag(pEntity, ssName);
	delete ssName;
	return bHasTag;
}

sledgelib_func char* Tag_Get(unsigned int iHandle, char* cName) {
	Entity* pEntity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!pEntity) {
		char* cRet = reinterpret_cast<char*>(SledgeLib::Interface->AllocateString(1));
		cRet[0] = '\0';
		return cRet;
	}
	/*
	 TO-DO:	the value returned by GetTag doesn't get disposed which results in a memory leak
			calling the destructor causes a heap corruption
	*/
	small_string* ssReturn = new small_string("null");
	small_string* ssName = new small_string(cName);

	Teardown::GetTag(pEntity, ssReturn, ssName);

	size_t lRetLen = ssReturn->len();
	char* pStringBuilder = reinterpret_cast<char*>(SledgeLib::Interface->AllocateString(lRetLen + 1));
	memcpy(pStringBuilder, ssReturn->c_str(), lRetLen);
	pStringBuilder[lRetLen] = '\0';

	return pStringBuilder;

}