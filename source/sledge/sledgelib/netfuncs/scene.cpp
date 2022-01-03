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

struct SParticleInfoNet {
	SParticleParamV4 m_Color;
	SParticleParamF m_Alpha;
	SParticleParamF m_Radius;
	SParticleParamF m_Gravity;
	SParticleParamF m_Drag;
	SParticleParamF m_Emissive;
	SParticleParamF m_Rotation;
	SParticleParamF m_Collide;
	SParticleParamF m_Stretch;
	SParticleParamF m_Sticky;
	unsigned int m_Tile;
	unsigned int m_Flags;

};

sledgelib_func void SpawnParticle(SParticleInfoNet NetParticleInfo, Vector3 vSpawnPos, Vector3 vVelocity, float fLifeTime) {
	SParticleInfo* ParticleInfo = reinterpret_cast<SParticleInfo*>(Teardown::alloc(sizeof(SParticleInfo)));

	ParticleInfo->m_Color = NetParticleInfo.m_Color;
	ParticleInfo->m_Alpha = NetParticleInfo.m_Alpha;
	ParticleInfo->m_Radius = NetParticleInfo.m_Radius;
	ParticleInfo->m_Gravity = NetParticleInfo.m_Gravity;
	ParticleInfo->m_Drag = NetParticleInfo.m_Drag;
	ParticleInfo->m_Emissive = NetParticleInfo.m_Emissive;
	ParticleInfo->m_Rotation = NetParticleInfo.m_Rotation;
	ParticleInfo->m_Collide = NetParticleInfo.m_Collide;
	ParticleInfo->m_Stretch = NetParticleInfo.m_Stretch;
	ParticleInfo->m_Sticky = NetParticleInfo.m_Sticky;

	ParticleInfo->m_Tile = NetParticleInfo.m_Tile;
	ParticleInfo->m_Flags = NetParticleInfo.m_Flags;

	Teardown::SpawnParticle(g_Game->m_Scene->m_SmokeParticles, ParticleInfo, &vSpawnPos, &vVelocity, fLifeTime);
}