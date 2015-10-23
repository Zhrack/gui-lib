#include "Gui.h"
#include "GuiEvent.h"
//#include "Widget.h"
//#include "Panel.h"
#include <assert.h>


namespace guiSystem
{
	Gui::Gui() :
		mFocusedWidget(nullptr),
		mOldMousePos(sf::Mouse::getPosition()),
		mRoot(nullptr)
	{

	}

	Gui::Gui(sf::RenderWindow* target) :
		mWindow(target),
		mFocusedWidget(nullptr),
		mOldMousePos(sf::Mouse::getPosition())
	{
		GuiContainer::Ptr temp(new GuiContainer(nullptr, this, "root"));
		mRoot = std::move(temp);
	}

	Gui::~Gui()
	{
	}

	void Gui::removeWidget(const Widget::Ptr& widget, bool recursive)
	{
		mRoot->removeChild(widget, recursive);
	}

	Panel::Ptr Gui::createPanel(const Widget::Ptr& parent, const std::string& name)
	{
		Panel::Ptr panel(new Panel(parent, this, name));
		parent->addChild(panel, name);
		return panel;
	}

	//convert from sf::Event to GuiEvent
	bool Gui::handleEvent(sf::Event& event)
	{
		GuiEvent guiEvent;
		bool onGUI = false;

		switch (event.type)
		{
		case sf::Event::Resized:
			//window resized, every widget should resize
			guiEvent.type = GuiEvent::Resized;
			guiEvent.size = event.size;

			// Run through widgets and resize them
			mRoot->resize(guiEvent);
			return true;
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::TextEntered:
			//send to focused
			guiEvent.type = GuiEvent::TextEntered;
			guiEvent.text = event.text;
			if (mFocusedWidget)
			{
				return mFocusedWidget->handleEvent(guiEvent);
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
				return mFocusedWidget->handleEvent(guiEvent);
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
				return mFocusedWidget->handleEvent(guiEvent);
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
			for (auto& widget : mRoot->mChildWidgets)
			{
				if (widget->mouseOnWidget(guiEvent.mouseButton.x, guiEvent.mouseButton.y))
				{
					// found it, focus and pass event
					this->changeFocus(widget);
					widget->handleEvent(guiEvent);
					return true;
				}
			}
			//click not on UI, unfocus and let event go
			this->noFocus();
			return false;

			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseButtonReleased:
			guiEvent.type = GuiEvent::MouseButtonReleased;
			guiEvent.mouseButton = event.mouseButton;

			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseMoved:
			guiEvent.type = GuiEvent::MouseMoved;
			guiEvent.mouseMove = event.mouseMove;

			if (mFocusedWidget && mFocusedWidget->isDraggable())
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					// Drag detected
					sf::Vector2i newMousePos(event.mouseMove.x, event.mouseMove.y);
					sf::Vector2i deltaPos = newMousePos - getOldMousePosition();

					mFocusedWidget->move(sf::Vector2f(deltaPos));
				}
			}

			updateMousePos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
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
		for (auto& widget : mRoot->mChildWidgets)
		{
			if (widget->handleEvent(guiEvent)) return true;
		}

		return false;
	}

	void Gui::draw() const
	{
		mWindow->draw(*mRoot);
	}

	void Gui::changeFocus(const Widget::Ptr& w)
	{
		assert(w != nullptr);

		noFocus();

		if (w->allowFocus())
		{
			mFocusedWidget = w;

			mFocusedWidget->focus();
		}
	}

	void Gui::noFocus()
	{
		if (mFocusedWidget)
			mFocusedWidget->unfocus();

		mFocusedWidget = nullptr;
	}
}