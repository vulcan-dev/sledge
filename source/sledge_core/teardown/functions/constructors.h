#pragma once

namespace Teardown {
	typedef void* (*tEntityConstructor) (void* pAlloc, void* pParent);

	inline tEntityConstructor BodyConstructor;
	inline tEntityConstructor ShapeConstructor;
	inline tEntityConstructor LightConstructor;
	//inline tEntityConstructor LocationConstructor;
	//inline tEntityConstructor WaterConstructor;
	//inline tClassConstructor Enemy; // rest in peace :(
	inline tEntityConstructor JointConstructor;
	//inline tEntityConstructor VehicleConstructor;
	//inline tEntityConstructor WheelConstructor;
	inline tEntityConstructor ScreenConstructor;
	//inline tEntityConstructor TriggerConstructor;
	inline tEntityConstructor ScriptConstructor;
}