#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "GuiEvent.h"

namespace gui
{
	struct ScrollBarCallbacks
	{
		int mCallbackIndices = -1;

		void OnUpLeftArrowClick(gui::GuiEvent& event, void* args);

		void OnDownRightArrowClick(gui::GuiEvent& event, void* args);

		void OnThumbDrag(gui::GuiEvent& event, void* args);
	};
} // namespace

#endif