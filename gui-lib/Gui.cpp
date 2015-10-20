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
		std::unique_ptr<Widget> temp(new Widget(nullptr, this, "root", sf::Vector2f(), mWindow->getSize(),
												true, true, false, false));
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
					this->changeFocus(widget);
					onGUI = true;
					break;
				}
			}
			//click not on UI, unfocus and let event go
			if (onGUI == false)
			{
				this->noFocus();
				return false;
			}

			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseButtonReleased:
			guiEvent.type = GuiEvent::MouseButtonReleased;
			guiEvent.mouseButton = event.mouseButton;

			//check to see if the click was on UI
			for (auto& widget : mRoot->mChildWidgets)
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
				this->noFocus();
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
		for (auto& widget : mRoot->mChildWidgets)
		{
			if (widget->handleEvent(guiEvent)) return true;
		}

		return false;
	}

	void Gui::draw() const
	{
		mRoot->draw(*mWindow, sf::RenderStates::Default);
	}

	//void Gui::changeFocus(Widget* const w)
	//{
	//	Widget::Ptr ptr = std::make_shared<Widget>(w);
	//	changeFocus(ptr);
	//}

	void Gui::changeFocus(const Widget::Ptr& w)
	{
		assert(w != nullptr);

		noFocus();

		mFocusedWidget = w;

		mFocusedWidget->focus();
	}

	void Gui::noFocus()
	{
		if (mFocusedWidget)
			mFocusedWidget->unfocus();

		mFocusedWidget = nullptr;
	}
}