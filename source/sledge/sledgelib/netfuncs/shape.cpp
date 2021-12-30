#include "teardown/classes/game.h"
#include "teardown/classes/entities.h"

#include "teardown/functions/memory.h"
#include "teardown/functions/constructors.h"

#include "teardown/utils.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func Transform GetShapeLocalTransform(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Body);
	if (!Shape) return Transform();
	return Shape->m_LocalTransform;
}

sledgelib_func void SetShapeLocalTransform(unsigned int iShapeHandle, Transform tLocalTransform) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Body);
	if (!Shape) return;
	Shape->m_LocalTransform = tLocalTransform;
}

sledgelib_func Transform GetShapeWorldTransform(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Body);
	if (!Shape) return Transform();
	return Transform(Shape->m_Position, Shape->m_Rotation);
}

sledgelib_func unsigned int GetShapeBody(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Body);
	if (!Shape) return 0;
	return Shape->m_Parent->m_Id;
}

sledgelib_func Bounds GetShapeBounds(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Body);
	if (!Shape) return Bounds();
	return Shape->m_Bounds;
}

sledgelib_func float GetShapeEmissiveScale(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Body);
	if (!Shape) return 0;
	return Shape->m_EmissiveScale;
}

sledgelib_func void SetShapeEmissiveScale(unsigned int iShapeHandle, float fValue) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Body);
	if (!Shape) return;
	Shape->m_EmissiveScale = fValue;
}

sledgelib_func bool GetShapeBroken(unsigned int iShapeHandle) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Body);
	if (!Shape) return false;
	return Shape->m_Broken;
}

sledgelib_func void SetShapeCollisionFilter(unsigned int iShapeHandle, unsigned char CollisionLayer, unsigned char CollisionMask) {
	CShape* Shape = Teardown::Utils::GetEntityByIdx<CShape*>(iShapeHandle, EEntityType::Body);
	if (!Shape) return;
	Shape->m_CollisionLayer = CollisionLayer;
	Shape->m_CollisionMask = CollisionMask;
}