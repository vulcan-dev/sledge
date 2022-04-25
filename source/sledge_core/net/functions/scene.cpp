#include "teardown/functions/scene.h"
#include "teardown/classes/game.h"
#include "teardown/classes/shape.h"
#include "teardown/types/math.h"
#include "teardown/types/misc.h"

#define sledgelib_func extern "C" __declspec(dllexport)

struct SRayInfo {
	bool m_Hit;
	float m_HitDist;
	Vector3 m_HitNormal;
	unsigned int m_HitShape;
};


sledgelib_func SRayInfo QueryRaycast(Vector3 vOrigin, Vector3 vDirection, float fMaxDist) {
	SRayInfo ReturnInfo;
	SRaycastFilter Filter;
	memset(&ReturnInfo, 0, sizeof(SRayInfo));
	memset(&Filter, 0, sizeof(SRaycastFilter));

	Filter.m_Mask = -1;
	
	Shape* HitShape;
	ReturnInfo.m_Hit = Teardown::QueryRaycast(g_Game->m_Scene, &vOrigin, &vDirection, fMaxDist, &Filter, &ReturnInfo.m_HitDist, &ReturnInfo.m_HitNormal, &HitShape, 0);

	if (ReturnInfo.m_Hit && HitShape)
		ReturnInfo.m_HitShape = HitShape->m_Id;

	return ReturnInfo;
}