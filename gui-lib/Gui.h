#ifndef GUI_H
#define GUI_H

//#include "Widget.h"
#include "Panel.h"

#include <vector>
#include <SFML\Window\Event.hpp>
#include <memory>
#include <string>

namespace guiSystem
{
	//class Widget;
	//class Widget::Ptr;
	//class Panel;

	class Gui
	{
	public:
		// Constructors
		Gui();

		Gui(sf::RenderWindow* target);

		//////////////////////////////////////////////////
		// Destructor
		~Gui();

		// Convert from sf::Event to GuiEvent
		bool handleEvent(sf::Event& event);

		// Manage focus changes
		void changeFocus(const Widget::Ptr& w);
		//void changeFocus(Widget* const w);
		void noFocus();

		// Getters and setters
		Widget::Ptr getFocusedWidget() const { return mFocusedWidget; }
		sf::RenderWindow* getWindow() const { return mWindow; }
		void setWindow(sf::RenderWindow* window) { mWindow = window; }
		const sf::Vector2i& getOldMousePosition() const { return mOldMousePos; }

		void updateMousePos(const sf::Vector2i& pos){ mOldMousePos = pos; }

		// Remove widget
		void removeWidget(const Widget::Ptr& widget, bool recursive = false);

		//////////////////////////////////////////////////
		///					WIDGET CREATION
		//////////////////////////////////////////////////
		//TODO implementare funzioni create man mano che vengono aggiunti widget

		Panel::Ptr createPanel(const Widget::Ptr& parent, const std::string& name);
		Panel::Ptr createPanel(const std::string& name){ return createPanel(mRoot, name); }

		// Draw all widgets
		void draw() const;

	private:
		// Currently focused widget, for keyboard events etc...
		Widget::Ptr mFocusedWidget;

		// Root of the widget tree. Every widget has this as parent if not specified
		// Its rect shape is as big as the screen
		Widget::Ptr mRoot;

		sf::RenderWindow* mWindow;

		sf::Vector2i mOldMousePos;
	};
}

#endif