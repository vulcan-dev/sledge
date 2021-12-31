#pragma once

namespace Teardown {
	namespace Constructors {
		typedef class CEntity* (*tClassConstructor) (void* pAlloc, CEntity* pParent);

		inline tClassConstructor Body;
		inline tClassConstructor Shape;
		inline tClassConstructor Light;
		inline tClassConstructor Location;
		inline tClassConstructor Water;
		inline tClassConstructor Enemy;
		inline tClassConstructor Joint;
		inline tClassConstructor Vehicle;
		inline tClassConstructor Wheel;
		inline tClassConstructor Screen;
		inline tClassConstructor Trigger;
		inline tClassConstructor Script;
	}
}