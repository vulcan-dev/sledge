#pragma once

#include <unordered_map>

#include "util/memory.h"

#define Sig(cName, cPattern, cMask) { cName, new CSignature(cPattern, cMask) },
#define RawSig(cName, cPattern, cMask) { cName, new CSignature(cPattern, cMask, false) },

inline std::unordered_map<const char*, CSignature*> g_SigMap = {
	/*
		memory
	*/
	Sig("free", "\xE8\x00\x00\x00\x00\xEB\xCE", "x????xx")
	Sig("malloc", "\xE8\x00\x00\x00\x00\xEB\x1F\x48\x85\xDB", "x????xxxxx")
	Sig("calloc", "\xE8\x00\x00\x00\x00\x33\xC9\x48\x8B\xF0", "x????xxxxx")
	Sig("realloc", "\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x85\xC0\x74\x16", "x????xxxxxxxx")
	Sig("memmove", "\xE8\x00\x00\x00\x00\x2B\xF5", "x????xx")

	/*
		registry
	*/
	Sig("SetBool", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\x97\xE8\x00\x00\x00\x00\x40\x84\xFF", "x????xxxxxx????xxx")
	Sig("SetInt", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\xF0\xE8\x00\x00\x00\x00\x33\xD2", "x????xxxxxx????xx")
	Sig("SetFloat", "\xE8\x00\x00\x00\x00\x90\xEB\x1F\xE8\x00\x00\x00\x00", "x????xxxx????")
	Sig("SetString", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\x58\xE8\x00\x00\x00\x00\x90", "x????xxxxxx????x")

	Sig("GetBool", "\xE8\x00\x00\x00\x00\x84\xC0\x74\x06\x44\x0F\xB6\xF6", "x????xxxxxxxx")
	Sig("GetInt", "\xE8\x00\x00\x00\x00\x89\x43\x2C", "x????xxx")
	Sig("GetFloat", "\xE8\x00\x00\x00\x00\x0F\x28\xC8\x0F\x28\xDF", "x????xxxxxx")
	Sig("GetString", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x55\x28\x49\x03\xD6", "x????xxxxxxxx")

	/*
		tags
	*/
	Sig("SetTag", "\xE8\x00\x00\x00\x00\xEB\x50\x48\x8D\x85\x00\x00\x00\x00", "x????xxxxx????")
	Sig("HasTag", "\xE8\x00\x00\x00\x00\x0F\xB6\xD8\x48\x8D\x4D\xD8", "x????xxxxxxx")
	Sig("RemoveTag", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x48\x8B\xFB", "x????xxxxx?x????xxx")
	Sig("GetTag", "\xE8\x00\x00\x00\x00\x90\x48\x8B\xC8\xE8\x00\x00\x00\x00\x41\x0F\x2E\xC3", "x????xxxxx????xxxx")

	/*
		constructors
	*/
	Sig("Game::Game", "\xE8\x00\x00\x00\x00\x4C\x8B\xF0\xEB\x03\x4D\x8B\xF7\x41\x8B\xF7", "x????xxxxxxxxxxx")

	Sig("Body::Body", "\xE8\x00\x00\x00\x00\x48\x8B\xF8\x33\xDB\xEB\x04", "x????xxxxxxx")
	Sig("Shape::Shape", "\xE8\x00\x00\x00\x00\x48\x8B\xF8\x66\x83\x4F\x00\x00", "x????xxxxxx??")

	/*
		events / callbacks
	*/
	Sig("SwitchGameState", "\xE8\x00\x00\x00\x00\x44\x89\x77\x0C", "x????xxxx")
	Sig("Update", "\xE8\x00\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x83\x3D\x00\x00\x00\x00\x00", "x????xxxx?x????xx?????")
	Sig("UpdatePlayer", "\xE8\x00\x00\x00\x00\x48\x8B\x4B\x78\xF3\x0F\x10\x8B\x00\x00\x00\x00", "x????xxxxxxxx????")
	Sig("ChangeLevel", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\x17\xE8\x00\x00\x00\x00\x48\x8D\x4D\x07", "x????xxxxxx????xxxx")

	/*
		patches
	*/
	Sig("IsWindowForegroundWindow", "\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x48\x8B\xD3\x49\x8B\xCE", "x????xxxx????xxxxxx")

	/*
		misc
	*/
	RawSig("Log", "\x80\x79\x0F\x00\x74\x03\x48\x8B\x09\x48\x8B\xD1\x48\x8D\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????")
	RawSig("DebugPrint", "\x44\x8B\x81\x00\x00\x00\x00\x4C\x8B\xD2", "xxx????xxx")
	RawSig("ScriptCore::RegisterLuaFunctions", "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x30\x48\x8D\x79\x30", "xxxx?xxxxxxxxx")
};