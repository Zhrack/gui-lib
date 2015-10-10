#ifndef WIDGET_H
#define WIDGET_H

#include <vector>
#include <string>
#include "GuiEvent.h"

#include "CallbackManager.h"

class Gui;

namespace guiSystem
{
	class Widget : public CallbackManager, public sf::Drawable
	{
	public:
		Widget();
		~Widget();

		void show()
		{
			enable();
			mVisible = true;
		}
		void hide()
		{
			disable();
			mVisible = false;
		}

		void enable(){ mEnabled = true; }
		void disable(){ mEnabled = false; }

		void focus();
		void unfocus();

		void setDraggable(bool draggable) { mDraggable = draggable; }

		//getters
		bool isEnabled() const { return mEnabled; }
		bool isVisible() const { return mVisible; }
		bool isFocused() const { return mFocused; }
		bool isDraggable() const { return mDraggable; }

		//check to see if mouse pointer is on this widget
		bool mouseOnWidget(float x, float y);

		Widget* getParent() const { return mParent; }

		bool handleEvent(GuiEvent& event);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) = 0;

	protected:
		void checkEventType(GuiEvent& event);

	protected:
		std::vector<std::string> mWidgetNames;
		std::vector<Widget*> mChildWidgets;

		Widget* mParent;

		Gui* mMainGui;

		//basic shape of the widget
		sf::RectangleShape mRect;

		//accepts events?
		bool mEnabled;
		//is rendered?
		bool mVisible;
		//is focused?
		bool mFocused;
		//is draggable?
		bool mDraggable;
	};
}

#endif