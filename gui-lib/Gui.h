#ifndef GUI_H
#define GUI_H

#include "Widget.h"

#include <vector>

namespace guiSystem
{
	class Gui
	{
	public:
		Gui();
		~Gui();

		//convert from sf::Event to GuiEvent
		bool handleEvent(sf::Event& event);
	private:
		std::vector<Widget*> mWidgets;

		Widget* mFocusedWidget;

		friend class Widget;
	};
}

#endif