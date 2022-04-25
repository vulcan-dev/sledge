#include "teardown/functions/constructors.h"
#include "teardown/functions/entity.h"
#include "teardown/classes/scene.h"
#include "teardown/classes/body.h"
#include "teardown/classes/shape.h"
#include "teardown/utils.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int Shape_Create(unsigned int iBodyHandle) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iBodyHandle, Type_Body);
	if (!pBody) return 0;

	void* pBuffer = Teardown::malloc(sizeof(Shape));
	if (pBuffer == NULL)
		return 0;

	Teardown::ShapeConstructor(pBuffer, pBody);
	Shape* pShape = reinterpret_cast<Shape*>(pBuffer);
	return pShape->m_Id;
}

sledgelib_func void Shape_Destroy(unsigned int iHandle) {
	Shape* pShape = Teardown::Utils::GetEntityByIdx<Shape*>(iHandle, Type_Shape);
	if (!pShape) return;
	pShape->Destroy(true);
}

sledgelib_func Transform Shape_GetLocalTransform(unsigned int iHandle) {
	Shape* pShape = Teardown::Utils::GetEntityByIdx<Shape*>(iHandle, Type_Shape);
	if (!pShape) return Transform();
	return pShape->m_LocalTransform;
}
sledgelib_func void Shape_SetLocalTransform(unsigned int iHandle, Transform tNew) {
	Shape* pShape = Teardown::Utils::GetEntityByIdx<Shape*>(iHandle, Type_Shape);
	if (!pShape) return;
	pShape->m_LocalTransform = tNew;
}

sledgelib_func float Shape_GetEmissive(unsigned int iHandle) {
	Shape* pShape = Teardown::Utils::GetEntityByIdx<Shape*>(iHandle, Type_Shape);
	if (!pShape) return 0.f;
	return pShape->m_Emissiveness;
}
sledgelib_func void Shape_SetEmissive(unsigned int iHandle, float fNew) {
	Shape* pShape = Teardown::Utils::GetEntityByIdx<Shape*>(iHandle, Type_Shape);
	if (!pShape) return;
	pShape->m_Emissiveness = fNew;
}

sledgelib_func Bounds Shape_GetBounds(unsigned int iHandle) {
	Shape* pShape = Teardown::Utils::GetEntityByIdx<Shape*>(iHandle, Type_Shape);
	if (!pShape) return Bounds();
	return Bounds(pShape->m_BoundsMin, pShape->m_BoundsMax);
}

sledgelib_func bool Shape_GetBroken(unsigned int iHandle) {
	Shape* pShape = Teardown::Utils::GetEntityByIdx<Shape*>(iHandle, Type_Shape);
	if (!pShape) return false;
	return pShape->m_Broken;
}

sledgelib_func bool Shape_LoadVox(unsigned int iHandle, char* cVoxPath, char* cObjectName, float fScale) {
	Shape* pShape = Teardown::Utils::GetEntityByIdx<Shape*>(iHandle, Type_Shape);
	if (!pShape) return false;
	if (pShape->m_Parent == NULL || pShape->m_Parent->m_Type != Type_Body) return false;

	small_string* ssPath = new small_string(cVoxPath);
	small_string* ssObject = new small_string(cObjectName);

	void* pVoxBackup = pShape->m_VoxBuffer;

	pShape->m_VoxBuffer = Teardown::LoadVox(ssPath, ssObject, fScale);
	if (!pShape->m_VoxBuffer) {
		pShape->m_VoxBuffer = pVoxBackup;
		return false;
	}

	Teardown::GenVoxTexture(pShape->m_VoxBuffer);
	Teardown::GenVoxBuffers(pShape->m_VoxBuffer);

	Body* pBody = reinterpret_cast<Body*>(pShape->m_Parent);

	pBody->m_Dynamic = true;
	pBody->m_ActiveTicksLeft = 60;
	
	Teardown::UpdateBodyActive(g_Scene->m_Physics, pBody);
	Teardown::InitializeBody(pBody);

	delete ssPath;
	delete ssObject;

	return true;
}