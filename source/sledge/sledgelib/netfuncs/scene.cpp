#include "teardown/functions/misc.h"
#include "teardown/classes/entities.h"
#include "teardown/classes/game.h"
#include "teardown/classes/misc.h"

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

sledgelib_func void Shoot(Vector3 vOrigin, Vector3 vVelocity, unsigned int iType, float fLifeTime, float fDamage) {
	SProjectile ProjectileInfo = *reinterpret_cast<SProjectile*>(Teardown::alloc(sizeof(SProjectile)));

	ProjectileInfo.m_Position = vOrigin;
	ProjectileInfo.m_Velocity = vVelocity;
	ProjectileInfo.m_DamageRadius = fDamage;
	ProjectileInfo.m_LifeTime = 0;
	ProjectileInfo.m_MaxLifeTime = fLifeTime;
	ProjectileInfo.m_Type = iType;

	g_Scene->m_Projectiles.push_back(ProjectileInfo);
}

sledgelib_func void DrawLine(Vector3 vPoint0, Vector3 vPoint1, Vector4 vColour, bool bIgnoreDepth) {
	Teardown::DrawLine(g_Game->m_Renderer, &vPoint0, &vPoint1, &vColour, &vColour, bIgnoreDepth);
}