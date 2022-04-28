#pragma once
class Body;
class Shape;
class Joint;
class small_string;
struct Vector3;

namespace Teardown {
	inline void (*GenVoxTexture)(void* pVox);
	inline void (*GenVoxBuffers)(void* pVox);

	inline void* (*LoadVox) (small_string* ssPath, small_string* ssObject, float fScale);

	inline void (*InitializeBody) (Body* pBody);
	inline void (*UpdateBodyActive) (void* pPhysics, Body* pBody);
	inline void (*InitializeBodyDynamic) (void* pPhysics, Body* pBody, bool bDynamic);
	inline void (*AttachJoint) (Joint* pJoint, Shape* pChild, Shape* pParent, Vector3* ChildJointPosLocal, Vector3* ParentJointPosLocal);
	inline void (*DetachJoint) (Joint* pJoint, Shape* pShape);
}