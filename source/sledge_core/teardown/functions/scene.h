#pragma once
class Scene;
struct Vector3;
struct SRaycastFilter;
class Shape;

namespace Teardown {
	inline bool (*QueryRaycast) (Scene* pScene, Vector3* vOrigin, Vector3* vDirection, float fMaxDist, SRaycastFilter* pFilter, float* fDistOut, Vector3* vHitPos, Shape** pHitShape, void* pUnk);
	inline bool (*MakeHole) (void* pUnknown, void* pUnknown2, Vector3* vPosition, float fSoftRadius, float fMidRadius, float fHardRadius, bool bSilent, void* pUnknown3);
}