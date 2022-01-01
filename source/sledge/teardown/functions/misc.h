#pragma once

#include "teardown/small_vector.h"
#include "teardown/classes/entities.h"
#include "teardown/classes/scene.h"
#include "util/math.h"

struct SRaycastFilter
{
	int m_Mask;
	int m_Layers;
	char field_8[4];
	bool m_IgnoreTransparent;
	small_vector<class CBody*> m_IgnoredBodies;
	char field_1D[32];
	small_vector<class CEntity*> m_IgnoredShapes;
};

namespace Teardown {
	typedef bool (*tQueryRaycast) (CScene* pScene, Vector3* vOrigin, Vector3* vDirection, float fMaxDist, void* pFilter, float* fDistOut, Vector3* vHitPos, CShape** pHitShape, void* pUnk);
	inline tQueryRaycast QueryRaycast;

	typedef void (*tDebugPrint) (void* m_Debug, small_string* ssMessage);
	inline tDebugPrint DebugPrint;
}