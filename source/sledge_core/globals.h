#pragma once

inline void* g_hSledge = nullptr;
inline void* g_hTeardown = nullptr;
inline void* g_hWnd = nullptr;

inline char g_ModulePath[261];

inline char g_Build[12] = __DATE__;

inline unsigned __int64 g_BaseAddress = 0;

inline bool g_SkipSplash = false;