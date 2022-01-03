#pragma once
enum class ECallbackType : unsigned int {
	PlayerSpawn,
	PreUpdate,
	PostUpdate,
	PrePlayerUpdate,
	PostPlayerUpdate,
	StateChange,
	Tick
};

typedef void (*tCallback) ();

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

	void OnStateChange(unsigned int iState);
	namespace StateChange {
		void RegisterCallback(CCallback*);
		void UnregisterCallback(CCallback*);
	}

	void OnTick();
	namespace Tick {
		void RegisterCallback(CCallback*);
		void UnregisterCallback(CCallback*);
	}
}

class CCallback {
private:
	ECallbackType m_Type;
public:
	bool m_Active;
	void* m_Func;

	CCallback() = delete;

	CCallback(ECallbackType eType, void* pFunc, bool bActive = true) {
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
		case ECallbackType::StateChange:
			_Callbacks::StateChange::RegisterCallback(this);
			break;
		case ECallbackType::Tick:
			_Callbacks::Tick::RegisterCallback(this);
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
		case ECallbackType::StateChange:
			_Callbacks::StateChange::UnregisterCallback(this);
			break;
		case ECallbackType::Tick:
			_Callbacks::Tick::UnregisterCallback(this);
			break;
		}
	}
};