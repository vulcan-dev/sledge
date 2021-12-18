#pragma once

namespace SledgeLib {
	namespace NetFuncs {
		/*
			 internal delegate void LogDelegate(ELogType LogType, string sMsg);
		*/
		void WriteLog(int eLogType, char* cMsg);

		/*
			internal delegate IntPtr CreateBindDelegate(EBindType eType, EKeyCode iKeyCode, BindCallback pCallback, bool bActive = true);
		*/
		void* CreateBind(unsigned int eType, int iKeyCode, void* pCallback, bool bActive);
	}
}