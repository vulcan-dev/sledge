#include "sledgelib/netfuncs.h"

#include "teardown/classes/game.h"

bool SledgeLib::NetFuncs::IsPlaying() {
	return g_Game->m_Playing;
}