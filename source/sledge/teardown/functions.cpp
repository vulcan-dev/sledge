#include "teardown/functions.h"
#include "teardown/signatures.h"

#include "teardown/functions/constructors.h"
#include "teardown/functions/memory.h"
#include "teardown/functions/registry.h"
#include "teardown/functions/entity.h"

#include "util/memory.h"
#include "util/log.h"

struct SScanneable {
	CSignature m_Sig;
	void* m_Function;
	const char* m_Name;
};

static const SScanneable aFunctions[] = {
	// memory
	{ Signatures::free, &Teardown::free, "free" },
	{ Signatures::alloc, &Teardown::alloc, "alloc" },
	{ Signatures::calloc, &Teardown::calloc, "calloc" },
	{ Signatures::realloc, &Teardown::realloc, "realloc" },
	{ Signatures::memmove, &Teardown::memmove, "memmove" },

	// registry
	{Signatures::SetBool, &Teardown::SetBool, "SetBool" },
	{Signatures::SetInt, &Teardown::SetInt, "SetInt" },
	{Signatures::SetFloat, &Teardown::SetFloat, "SetFloat" },
	{Signatures::SetString, &Teardown::SetString, "SetString" },

	{Signatures::GetBool, &Teardown::GetBool, "GetBool" },
	{Signatures::GetInt, &Teardown::GetInt, "GetInt" },
	{Signatures::GetFloat, &Teardown::GetFloat, "GetFloat" },
	{Signatures::GetString, &Teardown::GetString, "GetString" },

	// entity
	{Signatures::GenVoxPhysBuffer, &Teardown::Entity::GenVoxBuffers, "GenVoxBuffers" },
	{Signatures::GenVoxTexture, &Teardown::Entity::GenVoxTexture, "GenVoxTexture" },
	{Signatures::LoadVox, &Teardown::Entity::LoadVox, "LoadVox" },

	// constructors
	{ Signatures::Body, &Teardown::Constructors::Body, "CBody::CBody" },
	{ Signatures::Shape, &Teardown::Constructors::Shape, "CShape::CShape" },
	{ Signatures::Light, &Teardown::Constructors::Light, "CLight::CLight" },
	{ Signatures::Location, &Teardown::Constructors::Location, "CLocation::CLocation" },
	{ Signatures::Water, &Teardown::Constructors::Water, "CWater::CWater" },
	{ Signatures::Enemy, &Teardown::Constructors::Enemy, "CEnemy::CEnemy" },
	{ Signatures::Joint, &Teardown::Constructors::Joint, "CJoint::CJoint" },
	{ Signatures::Vehicle, &Teardown::Constructors::Vehicle, "CVehicle::CVehicle" },
	{ Signatures::Wheel, &Teardown::Constructors::Wheel, "CWheel::CWheel" },
	{ Signatures::Screen, &Teardown::Constructors::Screen, "CScreen::CScreen" },
	{ Signatures::Trigger, &Teardown::Constructors::Trigger, "CTrigger::CTrigger" },
	{ Signatures::Script, &Teardown::Constructors::Script, "CScript::CScript" },
};

void Teardown::GetFunctionAddresses() {
	for (const SScanneable &Scanneable : aFunctions) {
		unsigned long long dwFuncAddr = Memory::dwFindPattern(Scanneable.m_Sig);
		memcpy(Scanneable.m_Function, &dwFuncAddr, sizeof(void*));
		LogVerbose("{} - {}", Scanneable.m_Name, reinterpret_cast<void*>(dwFuncAddr));
	}
}