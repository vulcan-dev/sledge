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

struct Transform {
	Vector3 m_Position;
	Vector4 m_Rotation;
};

struct Bounds {
	Vector3 m_Min;
	Vector3 m_Max;
};