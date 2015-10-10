#include "Gui.h"
#include "GuiEvent.h"

namespace guiSystem
{
	Gui::Gui()
	{
	}


	Gui::~Gui()
	{
	}

	//convert from sf::Event to GuiEvent
	bool Gui::handleEvent(sf::Event& event)
	{
		GuiEvent guiEvent;

		switch (event.type)
		{
		case sf::Event::Closed:
			guiEvent.type = GuiEvent::Closed;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::Resized:
			guiEvent.type = GuiEvent::Resized;
			guiEvent.size = event.size;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::TextEntered:
			//send to focused
			guiEvent.type = GuiEvent::TextEntered;
			guiEvent.text = event.text;
			if (mFocusedWidget)
			{
				mFocusedWidget->handleEvent(guiEvent);
				return true;
			}
			else return false;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::KeyPressed:
			//send to focused
			guiEvent.type = GuiEvent::KeyPressed;
			guiEvent.key = event.key;
			if (mFocusedWidget)
			{
				mFocusedWidget->handleEvent(guiEvent);
				return true;
			}
			else return false;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::KeyReleased:
			//send to focused
			guiEvent.type = GuiEvent::KeyReleased;
			guiEvent.key = event.key;
			if (mFocusedWidget)
			{
				mFocusedWidget->handleEvent(guiEvent);
				return true;
			}
			else return false;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseWheelScrolled:
			guiEvent.type = GuiEvent::MouseWheelScrolled;
			guiEvent.mouseWheelScroll = event.mouseWheelScroll;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseButtonPressed:
			guiEvent.type = GuiEvent::MouseButtonPressed;
			guiEvent.mouseButton = event.mouseButton;

			//check to see if the click was on UI
			bool onGUI = false;
			for (auto widget : mWidgets)
			{
				if (widget->mouseOnWidget(guiEvent.mouseButton.x, guiEvent.mouseButton.y))
				{
					onGUI = true;
					break;
				}
			}
			//click not on UI, unfocus and let event go
			if (onGUI == false)
			{
				mFocusedWidget->unfocus();
				mFocusedWidget = nullptr;
				return false;
			}

			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseButtonReleased:
			guiEvent.type = GuiEvent::MouseButtonReleased;
			guiEvent.mouseButton = event.mouseButton;

			//check to see if the click was on UI
			bool onGUI = false;
			for (auto widget : mWidgets)
			{
				if (widget->mouseOnWidget(guiEvent.mouseButton.x, guiEvent.mouseButton.y))
				{
					onGUI = true;
					break;
				}
			}
			//click not on UI, unfocus and let event go
			if (onGUI == false)
			{
				mFocusedWidget->unfocus();
				mFocusedWidget = nullptr;
				return false;
			}

			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseMoved:
			guiEvent.type = GuiEvent::MouseMoved;
			guiEvent.mouseMove = event.mouseMove;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseEntered:
			guiEvent.type = GuiEvent::MouseEntered;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseLeft:
			guiEvent.type = GuiEvent::MouseLeft;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::JoystickButtonPressed:
			guiEvent.type = GuiEvent::JoystickButtonPressed;
			guiEvent.joystickButton = event.joystickButton;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::JoystickButtonReleased:
			guiEvent.type = GuiEvent::JoystickButtonReleased;
			guiEvent.joystickButton = event.joystickButton;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::JoystickMoved:
			guiEvent.type = GuiEvent::JoystickMoved;
			guiEvent.joystickMove = event.joystickMove;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::JoystickConnected:
			guiEvent.type = GuiEvent::JoystickConnected;
			guiEvent.joystickConnect = event.joystickConnect;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::JoystickDisconnected:
			guiEvent.type = GuiEvent::JoystickDisconnected;
			guiEvent.joystickConnect = event.joystickConnect;
			break;
			//////////////////////////////////////////////////////////////////////
		default:
			return false;
			break;
			//////////////////////////////////////////////////////////////////////
		}

		//send guiEvent to widgets
		for (auto widget : mWidgets)
		{
			if (widget->handleEvent(guiEvent)) return true;
		}

		return false;
	}
}