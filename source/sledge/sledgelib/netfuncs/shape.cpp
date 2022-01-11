#include "teardown/functions/constructors.h"
#include "teardown/functions/entity.h"
#include "teardown/classes/scene.h"
#include "teardown/utils.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int Shape_Create(unsigned int iBodyHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return 0;

	void* pBuffer = Teardown::alloc(sizeof(CShape));
	if (pBuffer == NULL)
		return 0;

	Teardown::Constructors::Shape(pBuffer, Body);
	CShape* Shape = reinterpret_cast<CShape*>(pBuffer);
	return Shape->m_Id;
}

sledgelib_func void Shape_Destroy(unsigned int iHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iHandle, EEntityType::Shape);
	if (!Shape) return;
	Shape->Destroy(true);
}

sledgelib_func Transform Shape_GetLocalTransform(unsigned int iHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iHandle, EEntityType::Shape);
	if (!Shape) return Transform();
	return Shape->m_LocalTransform;
}

sledgelib_func void Shape_SetLocalTransform(unsigned int iHandle, Transform tLocalTransform) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iHandle, EEntityType::Shape);
	if (!Shape) return;
	Shape->m_LocalTransform = tLocalTransform;
}


sledgelib_func float Shape_GetEmissiveScale(unsigned int iHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iHandle, EEntityType::Shape);
	if (!Shape) return 0;
	return Shape->m_EmissiveScale;
}

sledgelib_func void Shape_SetEmissiveScale(unsigned int iHandle, float fValue) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iHandle, EEntityType::Shape);
	if (!Shape) return;
	Shape->m_EmissiveScale = fValue;
}

sledgelib_func Bounds Shape_GetBounds(unsigned int iHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iHandle, EEntityType::Shape);
	if (!Shape) return Bounds();
	return Shape->m_Bounds;
}

sledgelib_func bool Shape_IsBroken(unsigned int iHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iHandle, EEntityType::Shape);
	if (!Shape) return false;
	return Shape->m_Broken;
}

sledgelib_func void Shape_SetCollisionFilter(unsigned int iHandle, unsigned char CollisionLayer, unsigned char CollisionMask) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iHandle, EEntityType::Shape);
	if (!Shape) return;
	Shape->m_CollisionLayer = CollisionLayer;
	Shape->m_CollisionMask = CollisionMask;
}


sledgelib_func bool Shape_LoadVox(unsigned int iHandle, char* cVoxPath, char* cObjectName, float fScale) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iHandle, EEntityType::Shape);
	if (!Shape) return false;
	if (Shape->m_Parent == NULL || Shape->m_Parent->m_Type != EEntityType::Body) return false;

	small_string ssPath(cVoxPath);
	small_string ssObjectName(cObjectName);
	CVox* pPrevVox = Shape->m_Vox;
	
	Shape->m_Vox = Teardown::Entity::LoadVox(&ssPath, &ssObjectName, fScale);
	
	if (!Shape->m_Vox) {
		Shape->m_Vox = pPrevVox;
		return false;
	}
	
	Teardown::Entity::GenVoxTexture(Shape->m_Vox);
	Teardown::Entity::GenVoxBuffers(Shape->m_Vox);
	
	CBody* Body = reinterpret_cast<CBody*>(Shape->m_Parent);

	Body->m_Dynamic = true;
	Body->m_ActiveTimeLeft = 60;
	Teardown::Entity::UpdateBodyDynamic(g_Scene->m_Physics, Body);

	Teardown::Entity::InitializeBody(reinterpret_cast<CBody*>(Shape->m_Parent));
	
	return true;
}