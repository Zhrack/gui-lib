#ifndef WIDGET_H
#define WIDGET_H

#include <vector>
#include <string>
#include <memory>
#include "GuiEvent.h"
#include "CallbackManager.h"

namespace gui
{
	class Gui;


	class Widget : public CallbackManager, public sf::Drawable
	{
	public:
		using Ptr = std::shared_ptr<Widget>;

		// Constructors
		Widget(Widget::Ptr parent, Gui* const gui, const std::string& name,
			const sf::Vector2f& pos, const sf::Vector2u& size,
			bool enabled, bool visible, bool focused, bool allowFocus, bool draggable);

		//////////////////////////////////////////////////
		// Destructor
		~Widget();

		void show()
		{
			mVisible = true;
		}
		void hide()
		{
			mVisible = false;
		}

		void enable()
		{ 
			mEnabled = true; 
			if (!isVisible())
				show();
		}
		void disable()
		{
			mEnabled = false;
			if (isVisible())
				hide();
		}

		void focus();
		void unfocus();

		void setFocusable(bool focusable) { mAllowFocus = focusable; }

		void setDraggable(bool draggable) { mDraggable = draggable; }
		void setDragging(bool drag) { mDragging = drag; }

		void setOrigin(const sf::Vector2f& origin){ mRect.setOrigin(origin); }

		void move(const sf::Vector2f& delta);
		// Set position. Local position to parent.
		virtual void setPosition(const sf::Vector2f& localPos);
		virtual void setPosition(float x, float y);
		// Set position with global coords
		virtual void setGlobalPosition(const sf::Vector2f& globalPos);
		virtual void setGlobalPosition(float x, float y);

		virtual void setSize(float x, float y);
		virtual void setSize(const sf::Vector2f& newSize);

		const sf::Vector2f& getPosition() const;
		const sf::Vector2f& getGlobalPosition() const;

		const std::string& getName() const { return mName; }
		sf::RectangleShape& getShape() { return mRect; }

		// Getters
		bool isEnabled() const { return mEnabled; }
		bool isVisible() const { return mVisible; }
		bool isFocused() const { return mFocused; }
		bool isDraggable() const { return mDraggable; }
		bool isDragging() const { return mDragging; }
		bool isDirty() const { return mDirty; }
		bool allowFocus() const { return mAllowFocus; }

		// Check to see if mouse pointer is on this widget
		bool mouseOnWidget(float x, float y);

		// Return a pointer to the widget the mouse is currently on
		void checkMouseEnterLeft(float x, float y);
		
		//TODO implement this
		void addChild(const Widget::Ptr& widget, const std::string& name);
		// return a widget given its name
		Widget::Ptr getChild(const std::string& name, bool recursive = false);

		bool removeChild(const Widget::Ptr& widget, bool recursive = false);
		

		Widget::Ptr getParent() const { return mParent; }
		Gui* getGui() const { return mMainGui; }

		bool handleEvent(GuiEvent& event);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
		
	protected:
		void checkEventType(GuiEvent& event);
		bool removeChild(Widget* widget, bool recursive = false);

		void checkMouseEntered(float x, float y);
		void checkMouseLeft(float x, float y);

		// Called when derived class must do some work, like in Label
		virtual void update();

		virtual void setDirty(bool recursive = false);
		void setClean();
	protected:
		std::vector<std::string> mWidgetNames;
		std::vector<Widget::Ptr> mChildWidgets;

		std::string mName;

		// Parent
		Widget::Ptr mParent;

		Gui* mMainGui;

		// Basic shape of the widget
		sf::RectangleShape mRect;

		// Accepts events?
		bool mEnabled;
		// Is rendered?
		bool mVisible;
		// Is focused?
		bool mFocused;
		// Is mouse over this widget?
		bool mMouseHover;
		// Is focusable?
		bool mAllowFocus;
		// Is draggable?
		bool mDraggable;
		// Is being dragged?
		bool mDragging;
		// Does it nedds an update?
		bool mDirty;

		friend class Gui;
	};
}

#endif