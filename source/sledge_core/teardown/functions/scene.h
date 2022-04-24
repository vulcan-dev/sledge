#pragma once

namespace Teardown {
	inline bool (*QueryRaycast) (class Scene* pScene, struct Vector3* vOrigin, Vector3* vDirection, float fMaxDist, struct SRaycastFilter* pFilter, float* fDistOut, Vector3* vHitPos, class Shape** pHitShape, void* pUnk);
}