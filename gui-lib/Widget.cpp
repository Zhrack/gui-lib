#include "Widget.h"
#include "Gui.h"

namespace guiSystem
{
	//void testFunc(GuiEvent& e, int i, float k){}
	////////////////////TEST
	//int a = 1;
	//float f = 2.3f;
	//std::function<void(GuiEvent&, int, float)> function = testFunc;
	//this->bindCallback(GuiEvent::Closed, function, a, f);

	////////////////////TEST

	Widget::Widget(Widget::Ptr const parent, Gui* const gui, const std::string& name,
		const sf::Vector2f& pos, const sf::Vector2u& size,
		bool enabled, bool visible, bool focused, bool draggable) 
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
		mDraggable(draggable)
	{
		setPosition(pos);
	}


	Widget::~Widget()
	{
		mParent->removeChild(this);

		mParent = nullptr;
		mMainGui = nullptr;
	}

	bool Widget::handleEvent(GuiEvent& event)
	{
		// Check children first
		for (auto& child : mChildWidgets)
		{
			if (child->handleEvent(event))
			{
				return true;
			}
		}

		// Children didn't process event, try this widget
		checkEventType(event);
		// If there are callbacks available
		if (mCallbacks[event.type].empty() == false)
		{
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
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::GainedFocus:
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
			if (mouseOnWidget(event.mouseButton.x, event.mouseButton.y))
			{
				// Focus this
				mMainGui->changeFocus(std::make_shared<Widget>(*this));
			}
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::MouseButtonReleased:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::MouseMoved:
			if (isFocused())
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (isDraggable())
					{
						// Drag detected
						sf::Vector2i newMousePos(event.mouseMove.x, event.mouseMove.y);
						sf::Vector2i deltaPos = newMousePos - mMainGui->getOldMousePosition();

						mRect.move(sf::Vector2f(deltaPos));
					}
				}
			}

			mMainGui->updateMousePos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::MouseEntered:
			break;
			//////////////////////////////////////////////////////////////////////
		case GuiEvent::MouseLeft:
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
}// namespace