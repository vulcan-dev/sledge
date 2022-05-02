#include "teardown/functions/scene.h"
#include "teardown/classes/scene.h"
#include "teardown/classes/game.h"
#include "teardown/classes/shape.h"
#include "teardown/types/math.h"
#include "teardown/types/misc.h"

#define sledgelib_func extern "C" __declspec(dllexport)

struct SQueryInfo {
	bool m_Hit;
	float m_HitDist;
	Vector3 m_HitNormal;
	Vector3 m_HitPos;
	Vector3 m_HitDirection;
	unsigned int m_HitShape;
};

/*
	TO-DO:
		Implement a way to query with filters from C#
*/
sledgelib_func SQueryInfo _QueryRaycast(Vector3 vOrigin, Vector3 vDirection, float fMaxDist) {
	SQueryInfo ReturnInfo;
	SQueryFilter Filter;
	memset(&ReturnInfo, 0, sizeof(SQueryInfo));
	memset(&Filter, 0, sizeof(SQueryFilter));

	Filter.m_Mask = -1;
	
	Shape* pHitShape;
	ReturnInfo.m_Hit = Teardown::QueryRaycast(g_Scene, &vOrigin, &vDirection, fMaxDist, &Filter, &ReturnInfo.m_HitDist, &ReturnInfo.m_HitNormal, &pHitShape, 0);

	if (ReturnInfo.m_Hit && pHitShape)
		ReturnInfo.m_HitShape = pHitShape->m_Id;

	return ReturnInfo;
}

sledgelib_func SQueryInfo _QuerySpherecast(Vector3 vOrigin, Vector3 vDirection, float fRadius, float fMaxDist) {
	SQueryInfo ReturnInfo;
	SQueryFilter Filter;
	memset(&ReturnInfo, 0, sizeof(SQueryInfo));
	memset(&Filter, 0, sizeof(SQueryFilter));

	Filter.m_Mask = -1;

	Shape* pHitShape;
	ReturnInfo.m_Hit = Teardown::QuerySpherecast(g_Scene, fRadius, &vOrigin, &vDirection, fMaxDist, &Filter, &ReturnInfo.m_HitDist, &ReturnInfo.m_HitNormal, &ReturnInfo.m_HitPos, &pHitShape, 0);

	if (ReturnInfo.m_Hit && pHitShape)
		ReturnInfo.m_HitShape = pHitShape->m_Id;

	return ReturnInfo;
}

sledgelib_func SQueryInfo _QueryClosestPoint(Vector3 vOrigin, float fMaxDist) {
	SQueryFilter Filter;
	SQueryInfo ReturnInfo;
	memset(&ReturnInfo, 0, sizeof(SQueryInfo));
	memset(&Filter, 0, sizeof(SQueryFilter));

	Filter.m_Mask = -1;

	Shape* pHitShape;
	ReturnInfo.m_Hit = Teardown::QueryClosestPoint(g_Scene, &vOrigin, fMaxDist, &Filter, &ReturnInfo.m_HitPos, &ReturnInfo.m_HitDirection, &pHitShape, 0);

	if (ReturnInfo.m_Hit && pHitShape)
		ReturnInfo.m_HitShape = pHitShape->m_Id;

	return ReturnInfo;
}

sledgelib_func unsigned int _MakeHole(Vector3 vPosition, float fSoftRadius, float fMidRadius, float fHardRadius, bool bSilent) {
	unsigned int iVoxCount = g_Scene->m_VoxCount;
	Teardown::MakeHole(g_Scene->m_Unknown, 0, &vPosition, fSoftRadius, fMidRadius, fHardRadius, bSilent, 0);
	return iVoxCount - g_Scene->m_VoxCount;
}

sledgelib_func void _DrawLine(Vector3 vOrigin, Vector3 vGoal, Vector4 vColour, bool bUseDepth) { Teardown::DrawLine(g_Game->m_Renderer, &vOrigin, &vGoal, &vColour, &vColour, bUseDepth); }
sledgelib_func void _DrawCross(Vector3 vPosition, Vector4 vColour, float fSize) {Teardown::DrawCross(g_Game->m_Renderer, &vPosition, &vColour, fSize);}

sledgelib_func void _Paint(Vector3 vPosition,unsigned int iType, float fRadius, float fProbability) { Teardown::Paint(g_Scene, &vPosition, fRadius, iType, fProbability); }