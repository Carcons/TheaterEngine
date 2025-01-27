#pragma once

#include "../Event/Event.hpp"

namespace Theater
{
	struct WinResizeEvent : Event
	{
		THT_EVENT_OBJ

		uint32_t NewWidth;
		uint32_t NewHeight;

		WinResizeEvent(uint32_t t_newWidth, uint32_t t_newHeight) :
			NewWidth(t_newWidth), NewHeight(t_newHeight)
		{}
	};

	struct WinMoveEvent : Event
	{
		THT_EVENT_OBJ

		uint32_t NewX;
		uint32_t NewY;

		WinMoveEvent(uint32_t t_newX, uint32_t t_newY) :
			NewX(t_newX), NewY(t_newY)
		{}
	};
}