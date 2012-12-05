#pragma once
#include "Event.h"

namespace Habanero
{
	namespace GUI
	{
		struct MouseEventArgs
		{
			MouseCodes::Button button;
			uint x, y;
			int delta;
		};
	}
}