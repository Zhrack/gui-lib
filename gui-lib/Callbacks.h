#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "GuiEvent.h"

namespace gui
{
	// All indices are negative. Positive indices are reserved for the user.

	struct BorderWidgetCallbacks
	{
		static const int mCloseButtonCallbackIndices = -1;

		static void OnMouseEntered(gui::GuiEvent& event, void* args);

		static void OnMouseLeft(gui::GuiEvent& event, void* args);

		static void OnMouseButtonDown(gui::GuiEvent& event, void* args);

		static void OnMouseButtonUp(gui::GuiEvent& event, void* args);
	};

	struct ScrollBarCallbacks
	{
		static const int mCallbackIndices = -1;

		static void OnUpLeftArrowClick(gui::GuiEvent& event, void* args);

		static void OnDownRightArrowClick(gui::GuiEvent& event, void* args);

		static void OnThumbDrag(gui::GuiEvent& event, void* args);
	};
} // namespace

#endif