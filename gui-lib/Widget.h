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

		void enable(){ mEnabled = true; show(); }
		void disable(){ mEnabled = false; hide(); }

		void focus();
		void unfocus();

		void setFocusable(bool focusable) { mAllowFocus = focusable; }

		void setDraggable(bool draggable) { mDraggable = draggable; }

		

		void setOrigin(const sf::Vector2f& origin){ mRect.setOrigin(origin); }

		void move(const sf::Vector2f& delta);
		// Set position. Local position to parent.
		//TODO limitare spostamento solo all'interno del padre
		void setPosition(const sf::Vector2f& localPos);
		// Set position with global coords
		void setGlobalPosition(const sf::Vector2f& globalPos);

		const sf::Vector2f& getPosition() const;
		const sf::Vector2f& getGlobalPosition() const;

		const std::string& getName() const { return mName; }
		sf::RectangleShape& getShape() { return mRect; }

		// Upon a window resize event, rescale this widget accordingly
		//TODO implement this
		//tutte le posizioni vengono mantenute anche in percentuale, in questo modo sono indipendente oppure guardare tgui
		void resize(GuiEvent& event){}

		// Getters
		bool isEnabled() const { return mEnabled; }
		bool isVisible() const { return mVisible; }
		bool isFocused() const { return mFocused; }
		bool isDraggable() const { return mDraggable; }
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

		// Called when derived class must do some work, like in TextWidget
		virtual void update(){}
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

		friend class Gui;
	};
}

#endif