#include "Widget.h"
#include "Gui.h"

namespace guiSystem
{
	Widget::Widget()
	{
	}


	Widget::~Widget()
	{
	}

	bool Widget::handleEvent(GuiEvent& event)
	{
		//TODO gestire focus, gestire drag del widget, keypress solo su focused
		
		//check children first
		for (auto& child : mChildWidgets)
		{
			if (child->handleEvent(event))
			{
				return true;
			}
		}

		//children didn't process event, try this widget
		checkEventType(event);
		//if there are 
		if (mCallbacks[event.type].empty() == false)
		{
			auto list = mCallbacks[event.type];
			for (auto& func : list)
			{
				func(event);
				//func = [&](GuiEvent& event){this->function(event); };
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
		case guiSystem::GuiEvent::Closed:
			break;
		case guiSystem::GuiEvent::Resized:
			break;
		case guiSystem::GuiEvent::LostFocus:
			break;
		case guiSystem::GuiEvent::GainedFocus:
			break;
		case guiSystem::GuiEvent::TextEntered:
			break;
		case guiSystem::GuiEvent::KeyPressed:
			break;
		case guiSystem::GuiEvent::KeyReleased:
			break;
		case guiSystem::GuiEvent::MouseWheelMoved:
			break;
		case guiSystem::GuiEvent::MouseWheelScrolled:
			break;
		case guiSystem::GuiEvent::MouseButtonPressed:
			if (mouseOnWidget(event.mouseButton.x, event.mouseButton.y))
			{
				if (mMainGui->mFocusedWidget)
					mMainGui->mFocusedWidget->unfocus();
				//focus this
				mMainGui->mFocusedWidget = this;
				focus();
			}
			break;
		case guiSystem::GuiEvent::MouseButtonReleased:
			break;
		case guiSystem::GuiEvent::MouseMoved:
			if (isFocused())
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					//drag detected
					sf::Vector2i newMousePos(event.mouseMove.x, event.mouseMove.y);
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{

				}
			}
			break;
		case guiSystem::GuiEvent::MouseEntered:
			break;
		case guiSystem::GuiEvent::MouseLeft:
			break;
		case guiSystem::GuiEvent::JoystickButtonPressed:
			break;
		case guiSystem::GuiEvent::JoystickButtonReleased:
			break;
		case guiSystem::GuiEvent::JoystickMoved:
			break;
		case guiSystem::GuiEvent::JoystickConnected:
			break;
		case guiSystem::GuiEvent::JoystickDisconnected:
			break;
		case guiSystem::GuiEvent::Count:
			break;
		default:
			break;
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
}//namespace

//enum EventType
//{
//	Closed,                 ///< The window requested to be closed (no data)
//	Resized,                ///< The window was resized (data in event.size)
//	LostFocus,              ///< The window lost the focus (no data)
//	GainedFocus,            ///< The window gained the focus (no data)
//	TextEntered,            ///< A character was entered (data in event.text)
//	KeyPressed,             ///< A key was pressed (data in event.key)
//	KeyReleased,            ///< A key was released (data in event.key)
//	MouseWheelMoved,        ///< The mouse wheel was scrolled (data in event.mouseWheel) (deprecated)
//	MouseWheelScrolled,     ///< The mouse wheel was scrolled (data in event.mouseWheelScroll)
//	MouseButtonPressed,     ///< A mouse button was pressed (data in event.mouseButton)
//	MouseButtonReleased,    ///< A mouse button was released (data in event.mouseButton)
//	MouseMoved,             ///< The mouse cursor moved (data in event.mouseMove)
//	MouseEntered,           ///< The mouse cursor entered the area of the window (no data)
//	MouseLeft,              ///< The mouse cursor left the area of the window (no data)
//	JoystickButtonPressed,  ///< A joystick button was pressed (data in event.joystickButton)
//	JoystickButtonReleased, ///< A joystick button was released (data in event.joystickButton)
//	JoystickMoved,          ///< The joystick moved along an axis (data in event.joystickMove)
//	JoystickConnected,      ///< A joystick was connected (data in event.joystickConnect)
//	JoystickDisconnected,   ///< A joystick was disconnected (data in event.joystickConnect)
//
//	Count                   ///< Keep last -- the total number of event types
//};