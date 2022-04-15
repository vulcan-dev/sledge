#pragma once
#include <unordered_map>

inline std::unordered_map<const char*, unsigned __int64> g_Addresses = {
	 {"SetBool", 0x000000000013A9B0 },
	 {"memmove", 0x0000000000290300 },
	 {"realloc", 0x00000000002F7470 },
	 {"free", 0x00000000002EDC54 },
	 {"SetInt", 0x000000000013AAC0 },
	 {"DebugPrint", 0x000000000010EF70 },
	 {"calloc", 0x00000000002F1134 },
	 {"malloc", 0x00000000002EDC94 },
	 {"GetFloat", 0x0000000000139B20 },
	 {"SetString", 0x000000000013AB30 },
	 {"SetFloat", 0x000000000013AA50 },
	 {"GetBool", 0x0000000000139A70 },
	 {"GetInt", 0x0000000000139B70 },
	 {"GetString", 0x0000000000139840 },
	 {"SetTag", 0x0000000000110630 },
	 {"HasTag", 0x00000000001102A0 },
	 {"RemoveTag", 0x00000000001103C0 },
	 {"GetTag", 0x0000000000110110 },
	 {"Game", 0x0000000000038CC0 },
	 {"Body", 0x00000000000BAC70 },
	 {"Shape", 0x000000000015DFA0 },
	 {"IsWindowForegroundWindow", 0x000000000016CD80 },
	 {"SwitchGameState", 0x000000000003E090 },
	 {"Log", 0x000000000016F330 },
};