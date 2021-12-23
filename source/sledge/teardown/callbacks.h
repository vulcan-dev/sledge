#pragma once

typedef void (*tCallbackFunction) (void);

enum class ECallbackType : unsigned int {
	PlayerSpawn,
	PreUpdate,
	PostUpdate,
	PrePlayerUpdate,
	PostPlayerUpdate
};


class CCallback;

namespace _Callbacks {
	void OnPreUpdate();
	namespace PreUpdate {
		void RegisterCallback(CCallback*);
		void UnregisterCallback(CCallback*);
	}

	void OnPostUpdate();
	namespace PostUpdate {
		void RegisterCallback(CCallback*);
		void UnregisterCallback(CCallback*);
	}

	void OnPlayerSpawn();
	namespace PlayerSpawn {
		void RegisterCallback(CCallback*);
		void UnregisterCallback(CCallback*);
	}

	void OnPrePlayerUpdate();
	namespace PrePlayerUpdate {
		void RegisterCallback(CCallback*);
		void UnregisterCallback(CCallback*);
	}

	void OnPostPlayerUpdate();
	namespace PostPlayerUpdate {
		void RegisterCallback(CCallback*);
		void UnregisterCallback(CCallback*);
	}
}

class CCallback {
private:
	ECallbackType m_Type;
public:
	bool m_Active;
	tCallbackFunction m_Func;

	CCallback() = delete;

	CCallback(ECallbackType eType, tCallbackFunction pFunc, bool bActive = true) {
		this->m_Type = eType; this->m_Func = pFunc; this->m_Active = bActive;

		switch (this->m_Type) {
		case ECallbackType::PlayerSpawn:
			_Callbacks::PlayerSpawn::RegisterCallback(this);
			break;
		case ECallbackType::PreUpdate:
			_Callbacks::PreUpdate::RegisterCallback(this);
			break;
		case ECallbackType::PostUpdate:
			_Callbacks::PostUpdate::RegisterCallback(this);
			break;
		case ECallbackType::PrePlayerUpdate:
			_Callbacks::PrePlayerUpdate::RegisterCallback(this);
			break;
		case ECallbackType::PostPlayerUpdate:
			_Callbacks::PostPlayerUpdate::RegisterCallback(this);
			break;
		}
	}

	~CCallback() {
		switch (this->m_Type) {
		case ECallbackType::PlayerSpawn:
			_Callbacks::PlayerSpawn::UnregisterCallback(this);
			break;
		case ECallbackType::PreUpdate:
			_Callbacks::PreUpdate::UnregisterCallback(this);
			break;
		case ECallbackType::PostUpdate:
			_Callbacks::PostUpdate::UnregisterCallback(this);
			break;
		case ECallbackType::PrePlayerUpdate:
			_Callbacks::PrePlayerUpdate::UnregisterCallback(this);
			break;
		case ECallbackType::PostPlayerUpdate:
			_Callbacks::PostPlayerUpdate::UnregisterCallback(this);
			break;
		}
	}
};