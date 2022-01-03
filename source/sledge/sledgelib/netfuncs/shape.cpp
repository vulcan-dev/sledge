#include "teardown/classes/game.h"
#include "teardown/classes/entities.h"

#include "teardown/functions/entity.h"
#include "teardown/functions/memory.h"
#include "teardown/functions/constructors.h"

#include "teardown/utils.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int CreateShape(unsigned int iBodyHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return 0;

	void* pShapeBuffer = Teardown::alloc(sizeof(CShape));
	if (pShapeBuffer == NULL)
		return 0;

	Teardown::Constructors::Shape(pShapeBuffer, Body);
	CShape* Shape = reinterpret_cast<CShape*>(pShapeBuffer);
	return Shape->m_Id;
}

sledgelib_func void DestroyShape(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return;
	Shape->Destroy(true);
}

sledgelib_func bool _LoadVox(unsigned int iShapeHandle, char* cVoxPath, char* cObjectName, float fScale) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
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

	Teardown::Entity::SetBodyDynamic(reinterpret_cast<CBody*>(Shape->m_Parent), true);
	Teardown::Entity::InitializeBody(reinterpret_cast<CBody*>(Shape->m_Parent));
	
	return true;
}

sledgelib_func Transform GetShapeLocalTransform(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return Transform();
	return Shape->m_LocalTransform;
}

sledgelib_func void SetShapeLocalTransform(unsigned int iShapeHandle, Transform tLocalTransform) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return;
	Shape->m_LocalTransform = tLocalTransform;
}

sledgelib_func Transform GetShapeWorldTransform(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return Transform();
	return Transform(Shape->m_Position, Shape->m_Rotation);
}

sledgelib_func unsigned int GetShapeBody(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return 0;
	return Shape->m_Parent->m_Id;
}

sledgelib_func Bounds GetShapeBounds(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return Bounds();
	return Shape->m_Bounds;
}

sledgelib_func float GetShapeEmissiveScale(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return 0;
	return Shape->m_EmissiveScale;
}

sledgelib_func void SetShapeEmissiveScale(unsigned int iShapeHandle, float fValue) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return;
	Shape->m_EmissiveScale = fValue;
}

sledgelib_func bool GetShapeBroken(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return false;
	return Shape->m_Broken;
}

sledgelib_func void SetShapeCollisionFilter(unsigned int iShapeHandle, unsigned char CollisionLayer, unsigned char CollisionMask) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return;
	Shape->m_CollisionLayer = CollisionLayer;
	Shape->m_CollisionMask = CollisionMask;
}

sledgelib_func unsigned int GetShapeSibling(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Shape);
	if (!Shape) return 0;
	
	if (!Shape->m_Sibling)
		return 0;

	return Shape->m_Sibling->m_Id;
}