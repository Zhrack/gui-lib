#ifndef GUI_H
#define GUI_H

//#include "Widget.h"

#include <vector>
#include <SFML\Window\Event.hpp>
#include <memory>
#include <string>

namespace guiSystem
{
	class Widget;

	class Gui
	{
	public:
		// Constructors
		Gui();

		Gui(std::shared_ptr<sf::Window> win);

		//////////////////////////////////////////////////
		// Destructor
		~Gui();

		// Convert from sf::Event to GuiEvent
		bool handleEvent(sf::Event& event);

		// Manage focus changes
		void changeFocus(Widget* const w);
		void noFocus();

		// Getters and setters
		Widget* getFocusedWidget() const { return mFocusedWidget; }
		sf::Window* getWindow() const { return mWindow.lock().get(); }
		const sf::Vector2i& getOldMousePosition() const { return mOldMousePos; }

		void updateMousePos(const sf::Vector2i& pos){ mOldMousePos = pos; }

		//////////////////////////////////////////////////
		///					WIDGET CREATION
		//////////////////////////////////////////////////
		//TODO implementare funzioni create man mano che vengono aggiunti widget
		void createPanel(Widget* const parent, const std::string& name){}

	private:
		// Currently focused widget, for keyboard events etc...
		Widget* mFocusedWidget;

		// Root of the widget tree. Every widget has this as parent if not specified
		// Its rect shape is as big as the screen
		std::unique_ptr<Widget> mRoot;

		std::weak_ptr<sf::Window> mWindow;

		sf::Vector2i mOldMousePos;
	};
}

#endif