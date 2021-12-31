#pragma once

namespace Teardown {
	namespace Constructors {
		typedef void* (*tClassConstructor) (void* pAlloc, void* pParent);

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