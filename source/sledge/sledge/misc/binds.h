#include "util/keycodes.h"

enum class EBindType : unsigned char {
	Callback,
	Increase,
	Decrease,
	BoolToggle,
	Bool
};

class CKeyBind {
public:
	CKeyBind(EBindType eType, int iKeyCode, void* pValue, bool bActive = true);
	~CKeyBind();
	void OnKeyDown();
	void OnKeyUp();

	EBindType m_BindType;
	unsigned __int64 m_KeyId;
	bool m_Active;
	void* m_Value;
};

namespace Binds {
	void OnInput(unsigned int uMsg, unsigned __int64 wParam);
}