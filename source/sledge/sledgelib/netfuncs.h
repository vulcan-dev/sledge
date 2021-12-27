#pragma once

namespace SledgeLib {
	namespace NetFuncs {
		/*
			 internal delegate void dLog(ELogType LogType, string sMsg);
		*/
		void WriteLog(int eLogType, char* cMsg);

		/*
			internal delegate IntPtr dRegisterInputReader(dCallback Callback);
		*/
		void RegisterInputReader(void* InputReader);

		/*
			internal delegate IntPtr dRegisterCallback(ECallbackType eType, CallbackDelegate pCallback);
		*/
		void RegisterCallback(unsigned int eType, void* pCallback);

		/*
			internal delegate IntPtr dGetPlayer();
		*/
		void* GetPlayer();
	}
}