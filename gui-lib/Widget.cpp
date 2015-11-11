#include "Widget.h"
#include "Gui.h"

#include <iostream>

namespace gui
{
	Widget::Widget(Widget::Ptr parent, Gui* const gui, const std::string& name,
		const sf::Vector2f& pos, const sf::Vector2u& size,
		bool enabled, bool visible, bool focused, bool allowFocus, bool draggable)
		:
		mParent(parent),
		mMainGui(gui),
		mChildWidgets(),
		mWidgetNames(),
		mName(name),
		mRect(sf::Vector2f(size)),

		mEnabled(enabled),
		mVisible(visible),
		mFocused(focused),
		mAllowFocus(allowFocus),
		mMouseHover(false),
		mDraggable(draggable)
	{
		setGlobalPosition(pos);
	}


	Widget::~Widget()
	{
		mParent->removeChild(this);

		mParent = nullptr;
		mMainGui = nullptr;
	}

	bool Widget::handleEvent(GuiEvent& event)
	{
		// These events must be processed only locally
		if (event.type != GuiEvent::MouseEntered &&
			event.type != GuiEvent::MouseLeft &&
			event.type != GuiEvent::GainedFocus && 
			event.type != GuiEvent::LostFocus)
		{
			 // Check children first
			for (auto& child : mChildWidgets)
			{
				if (child->handleEvent(event))
				{
					return true;
				}
			}
		}

		// Children didn't process event, try this widget
		checkEventType(event);
		// If there are callbacks available
		if (mCallbacks[event.type].empty() == false)
		{
			switch (event.type)
			{
			case GuiEvent::MouseButtonPressed:
			case GuiEvent::MouseButtonReleased:
				if (mouseOnWidget(event.mouseButton.x, event.mouseButton.y) == false)
					return false;
				break;
			case GuiEvent::MouseMoved:
				if (mouseOnWidget(event.mouseMove.x, event.mouseMove.y) == false)
					return false;
				break;
			case GuiEvent::MouseEntered:
				if (mMouseHover == true)
					return false;
				break;
			case GuiEvent::MouseLeft:
				if (mMouseHover == false)
					return false;
				break;
			default:
				break;
			}

			auto list = mCallbacks[event.type];
			for (auto& func : list)
			{
				func(event);
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Widget::mouseOnWidget(float x, float y)
	{
		sf::FloatRect rect = mRect.getGlobalBounds();
		if (rect.contains(x, y)) return true;

		return false;
	}

	void Widget::checkEventType(GuiEvent& event)
	{
		switch (event.type)
		{
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::Resized:
			resize(event);
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::LostFocus:
			std::cout << getName() << ": lost focus" << std::endl;
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::GainedFocus:
			std::cout << getName() << ": gained focus" << std::endl;
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::TextEntered:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::KeyPressed:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::KeyReleased:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::MouseWheelScrolled:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::MouseButtonPressed:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::MouseButtonReleased:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::MouseMoved:
			// Some widgets need to update derived members data
			update();
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::MouseEntered:
			std::cout << getName() << ": mouse entered" << std::endl;
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::MouseLeft:
			std::cout << getName() << ": mouse left" << std::endl;
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::JoystickButtonPressed:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::JoystickButtonReleased:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::JoystickMoved:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::JoystickConnected:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::JoystickDisconnected:
			break;
			//////////////////////////////////////////////////////////////////////
		default:
			break;
			//////////////////////////////////////////////////////////////////////
		}
	}

	void Widget::addChild(const Widget::Ptr& widget, const std::string& name)
	{
		mChildWidgets.push_back(widget);
		mWidgetNames.push_back(name);
	}

	Widget::Ptr Widget::getChild(const std::string& name, bool recursive)
	{
		for (auto& widget : mChildWidgets)
		{
			if (widget->getName() == name)
			{
				return widget;
			}
		}

		if (recursive)
		{
			for (auto& widget : mChildWidgets)
			{
				auto& w = widget->getChild(name, true);
				if (w != nullptr)
					return w;
			}
		}

		return nullptr;
	}

	bool Widget::removeChild(const Widget::Ptr& widget, bool recursive)
	{
		return removeChild(widget.get(), recursive);
	}

	bool Widget::removeChild(Widget* widget, bool recursive)
	{
		auto& w = mChildWidgets.begin();
		for (int i = 0; w != mChildWidgets.end(); ++w, ++i)
		{
			if (w->get() == widget)
			{
				mWidgetNames.erase(mWidgetNames.begin() + i);
				mChildWidgets.erase(w);
				return true;
			}
		}

		if (recursive)
		{
			for (auto& w : mChildWidgets)
			{
				if (w->removeChild(widget, true))
					return true;
			}
		}

		return false;
	}

	void Widget::move(const sf::Vector2f& delta)
	{
		sf::Vector2f clampedDelta = delta;
		sf::Vector2f resultPos = getGlobalPosition() + delta;
		sf::FloatRect rect = mParent->getShape().getGlobalBounds();
		sf::Vector2f size = mRect.getSize();
		GuiContainer::Ptr root = mMainGui->getRoot();

		if (resultPos.x < rect.left) // correct on left
		{
			resultPos.x = rect.left;
			clampedDelta.x = 0;
		}
		else if (resultPos.x + size.x > rect.left + rect.width) // correct on right
		{
			resultPos.x = rect.left + rect.width - size.x;
			clampedDelta.x = 0;
		}

		if (resultPos.y < rect.top) // correct on top
		{
			resultPos.y = rect.top;
			clampedDelta.y = 0;
		}
		else if (resultPos.y + size.y > rect.top + rect.height) // correct on bottom
		{
			resultPos.y = rect.top + rect.height - size.y;
			clampedDelta.y = 0;
		}

		setGlobalPosition(resultPos);

		for (auto& child : mChildWidgets)
		{
			child->move(clampedDelta);
		}
	}

	void Widget::checkMouseEnterLeft(float x, float y)
	{
		// Loop through all widgets
		for (auto& it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
		{
			// Check if the widget is visible and enabled
			if ((*it)->isEnabled())
			{
				// Create MouseEntered event if the mouse is on top of it and mMouseHover == false
				if ((*it)->mouseOnWidget(x, y))
				{
					(*it)->checkMouseEntered(x, y);
				}
				else
				{
					(*it)->checkMouseLeft(x, y);
				}

				(*it)->checkMouseEnterLeft(x, y);
			}
		}
	}

	void Widget::checkMouseEntered(float x, float y)
	{
		if (mMouseHover == false)
		{
			mMouseHover = true;
			GuiEvent event;
			event.type = GuiEvent::MouseEntered;
			event.mouseEnterLeft.entered = true;
			event.mouseEnterLeft.x = x;
			event.mouseEnterLeft.y = y;
			this->handleEvent(event);
		}
	}

	void Widget::checkMouseLeft(float x, float y)
	{
		if (mMouseHover)
		{
			mMouseHover = false;
			GuiEvent event;
			event.type = GuiEvent::MouseLeft;
			event.mouseEnterLeft.entered = false;
			event.mouseEnterLeft.x = x;
			event.mouseEnterLeft.y = y;
			this->handleEvent(event);
		}
	}

	void Widget::focus()
	{
		mFocused = true;
		GuiEvent e;
		e.type = GuiEvent::GainedFocus;
		this->handleEvent(e);
	}
	void Widget::unfocus()
	{
		mFocused = false;
		GuiEvent e;
		e.type = GuiEvent::LostFocus;
		this->handleEvent(e);
	}

	// Set position. Local position to parent.
	//TODO limitare spostamento solo all'interno del padre
	void Widget::setPosition(const sf::Vector2f& pos)
	{
		if (mParent)
			mRect.setPosition(pos + mParent->getGlobalPosition());

		//mRect.setPosition(pos);
	}

	// Set position with global coords
	void Widget::setGlobalPosition(const sf::Vector2f& pos)
	{ 
		mRect.setPosition(pos);
		//if (mParent)
		//	mRect.setPosition(pos - mParent->getGlobalPosition());
	}

	const sf::Vector2f& Widget::getPosition() const 
	{ 
		if (mParent)
			return mRect.getPosition() - mParent->getGlobalPosition();

		// GuiContainer
		return sf::Vector2f();

		//return mRect.getPosition();
	}

	const sf::Vector2f& Widget::getGlobalPosition() const 
	{ 
		return mRect.getPosition(); 

		//if (mParent)
		//	return mRect.getPosition() + mParent->getGlobalPosition();

		//return sf::Vector2f();
	}
}// namespace