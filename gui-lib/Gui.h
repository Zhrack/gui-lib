#ifndef GUI_H
#define GUI_H

//#include "Widget.h"

#include <vector>
#include <SFML\Window\Event.hpp>

namespace guiSystem
{
	class Widget;

	class Gui
	{
	public:
		Gui();
		~Gui();

		// Convert from sf::Event to GuiEvent
		bool handleEvent(sf::Event& event);

		// Manage focus changes
		void changeFocus(Widget* w);
		void noFocus();

		// Getters and setters
		Widget* getFocusedWidget() const { return mFocusedWidget; }
		sf::Window* getWindow() const { return mWindow; }
		const sf::Vector2i& getOldMousePosition() const { return mOldMousePos; }

		void updateMousePos(const sf::Vector2i& pos){ mOldMousePos = pos; }

	private:
		std::vector<Widget*> mWidgets;

		// Currently focused widget, for keyboard events etc...
		Widget* mFocusedWidget;

		// Root of the widget tree. Every widget in mWidgets has this as parent.
		// Its rect shape is as big as the screen
		Widget* mRoot;

		sf::Window* mWindow;

		sf::Vector2i mOldMousePos;

		friend class Widget;
	};
}

#endif