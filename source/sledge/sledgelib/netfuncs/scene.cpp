#include "teardown/functions/misc.h"
#include "teardown/classes/entities.h"
#include "teardown/classes/game.h"

#include "util/math.h"

#define sledgelib_func extern "C" __declspec(dllexport)

struct SRaycastReturn {
	bool m_Hit;
	float m_HitDist;
	Vector3 m_HitNormal;
	unsigned int m_HitShape;
};

sledgelib_func SRaycastReturn QueryRaycast(Vector3 vOrigin, Vector3 vDirection, float fMaxDist) {
	SRaycastReturn Return;
	SRaycastFilter Filter;
	memset(&Return, 0, sizeof(SRaycastReturn));
	memset(&Filter, 0, sizeof(SRaycastFilter));

	Filter.m_Mask = -1;

	Vector3 vPos = vOrigin;
	Vector3 vDir = vDirection;

	CShape* HitShape = nullptr;
	float fDist;
	Vector3 vHitNormal;
	bool bHit = Teardown::QueryRaycast(g_Game->m_Scene, &vPos, &vDir, fMaxDist, &Filter, &fDist, &vHitNormal, &HitShape, 0);

	Return.m_Hit = bHit;

	if (bHit) {
		if (HitShape)
			Return.m_HitShape = HitShape->m_Id;
		Return.m_HitNormal = vHitNormal;
		Return.m_HitDist = fDist;
	}

	return Return;
}