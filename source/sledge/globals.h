#pragma once

inline char g_Build[12] = __DATE__;
inline char g_ModulePath[261];
inline void* g_Module;

inline void* g_hMod;

inline void* g_hWnd;
inline int g_WindowWidth;
inline int g_WindowHeight;
inline bool g_WindowReady = false;