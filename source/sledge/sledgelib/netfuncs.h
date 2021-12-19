#pragma once

namespace SledgeLib {
	namespace NetFuncs {
		/*
			 internal delegate void LogDelegate(ELogType LogType, string sMsg);
		*/
		void WriteLog(int eLogType, char* cMsg);

		/*
			internal delegate IntPtr CreateBindDelegate(EKeyCode iKeyCode, CallbackDelegate pCallback, bool bActive = true);
		*/
		void* CreateBind(int iKeyCode, void* pCallback, bool bActive);

		/*
			internal delegate IntPtr CreateCallbackDelegate(ECallbackType eType, CallbackDelegate pCallback, bool bActive = true);
		*/
		void* CreateCallback(unsigned int eType, void* pCallback, bool bActive);

		/*
			internal delegate bool IsPlayingDelegate();
		*/
		bool IsPlaying();

		/*
			internal delegate IntPtr GetPlayerDelegate();
		*/
		void* GetPlayer();
	}
}