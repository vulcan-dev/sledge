#pragma once

#include "teardown/types/small_vector.h"

struct SQueryFilter
{
	int m_Mask;
	int m_Layers;
	char field_8[4];
	bool m_IgnoreTransparent;
	small_vector<class CBody*> m_IgnoredBodies;
	char field_1D[32];
	small_vector<class CEntity*> m_IgnoredShapes;
};