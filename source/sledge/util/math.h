#pragma once

struct Vector2 {
	float x, y;
};

struct Vector3 {
	float x, y, z;
};

struct Vector4 {
	float x, y, z, w;
};

struct Quaternion {
	float x, y, z, w;
};

struct Transform {
	Vector3 m_Position;
	Quaternion m_Rotation;
};

struct Bounds {
	Vector3 m_Min;
	Vector3 m_Max;
};

inline Vector3 V3Add(Vector3 v1, Vector3 v2) {
	return Vector3(v1.x + v2.x, v2.y + v2.y, v2.x + v2.y);
}

inline Quaternion QuatMul(Quaternion q1, Quaternion q2) {
	return Quaternion(q1.y * q2.z - q1.z * q2.y + q1.x * q2.w + q1.w * q2.x,
		q1.z * q2.x - q1.x * q2.z + q1.y * q2.w + q1.w * q2.y,
		q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
}

inline Transform TrAdd(Transform t1, Transform t2) {
	Transform Result;
	Result.m_Rotation = QuatMul(t1.m_Rotation, t2.m_Rotation);
	Result.m_Position = V3Add(t1.m_Position, t2.m_Position);
	return Result;
}