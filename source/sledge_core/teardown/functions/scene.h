#pragma once
class Scene;
struct Vector3;
struct Vector4;
struct SQueryFilter;
class Shape;

namespace Teardown {
	inline bool (*QueryRaycast) (Scene* pScene, Vector3* vOrigin, Vector3* vDirection, float fMaxDist, SQueryFilter* pFilter, float* fDistOut, Vector3* vHitPos, Shape** pHitShape, void* pUnk);
	inline bool (*QuerySpherecast) (Scene* pScene, float fRadius, Vector3* vOrigin, Vector3* vDirection, float fMaxDist, SQueryFilter* pFilter, float* fDistOut, Vector3* vNormal, Vector3* vHitPos, Shape** pHitShape, void* pUnk2);
	inline bool (*QueryClosestPoint) (Scene* pScene, Vector3* vOrigin, float fMaskDist, SQueryFilter* pFilter, Vector3* vHitPos, Vector3* vHitNormal, Shape** pHitShape, void* pUnk);

	inline bool (*MakeHole) (void* pUnknown, void* pUnknown2, Vector3* vPosition, float fSoftRadius, float fMidRadius, float fHardRadius, bool bSilent, void* pUnknown3);
	inline void (*DrawLine) (void* pRenderer, Vector3* vOrigin, void* pGoal, Vector4* vColour, Vector4* vColour2, bool bDepthCheck);
}