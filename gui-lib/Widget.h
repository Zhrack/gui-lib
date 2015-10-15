#ifndef WIDGET_H
#define WIDGET_H

#include <vector>
#include <string>
#include "GuiEvent.h"
#include "CallbackManager.h"

namespace guiSystem
{
	class Gui;


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

		// Set position. Local position to parent.
		void setPosition(const sf::Vector2f& p){ mRect.setPosition(p); }
		// Set position with global coords
		void setGlobalPosition(const sf::Vector2f& p){ setPosition(p - mParent->getGlobalPosition()); }

		void setOrigin(const sf::Vector2f& o){ mRect.setOrigin(o); }

		const sf::Vector2f& getPosition() const { return mRect.getPosition(); }
		const sf::Vector2f& getGlobalPosition() const { return mParent->getGlobalPosition() + getPosition(); }

		// Upon a window resize event, rescale this widget accordingly
		//TODO implement this
		//tutte le posizioni vengono mantenute anche in percentuale, in questo modo sono indipendente oppure guardare tgui
		void resize(GuiEvent& event){}

		// Getters
		bool isEnabled() const { return mEnabled; }
		bool isVisible() const { return mVisible; }
		bool isFocused() const { return mFocused; }
		bool isDraggable() const { return mDraggable; }

		// Check to see if mouse pointer is on this widget
		bool mouseOnWidget(float x, float y);
		

		Widget* getParent() const { return mParent; }

		bool handleEvent(GuiEvent& event);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) {}

	protected:
		void checkEventType(GuiEvent& event);

	protected:
		std::vector<std::string> mWidgetNames;
		std::vector<Widget*> mChildWidgets;

		Widget* mParent;

		Gui* mMainGui;

		// Basic shape of the widget
		sf::RectangleShape mRect;

		// Accepts events?
		bool mEnabled;
		// Is rendered?
		bool mVisible;
		// Is focused?
		bool mFocused;
		// Is draggable?
		bool mDraggable;
	};
}

#endif