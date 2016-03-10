#include "Gui.h"

#include "GuiEvent.h"
#include <assert.h>
#include <iostream>
#include <fstream>

namespace gui
{
	Gui::Gui() :
		mFocusedWidget(nullptr),
		mOldMousePos(sf::Mouse::getPosition()),
		mRoot(nullptr),
		mDefaultFont("DejaVuSans"),
		dirtyScissor(true)
	{
		GuiContainer::Ptr temp(new GuiContainer(nullptr, this, "root"));
		mRoot = std::move(temp);
		mRoot->setChildrenOut(false);

		resetScissor();

		sf::Font font;
		if (!font.loadFromFile("fonts/DejaVuSans.ttf"))
		{
			std::cout << "font load failed" << std::endl;
		}
		mFonts[mDefaultFont] = font;
	}

	Gui::Gui(sf::RenderWindow* target) :
		mWindow(target),
		mFocusedWidget(nullptr),
		mOldMousePos(sf::Mouse::getPosition()),
		mDefaultFont("DejaVuSans"),
		dirtyScissor(true)
	{
		GuiContainer::Ptr temp(new GuiContainer(nullptr, this, "root"));
		mRoot = std::move(temp);
		mRoot->getShape().setSize(sf::Vector2f(this->mWindow->getSize()));

		mRoot->setChildrenOut(false);

		resetScissor();

		sf::Font font;
		if (!font.loadFromFile("fonts/DejaVuSans.ttf"))
		{
			std::cout << "font load failed" << std::endl;
		}
		mFonts[mDefaultFont] = font;
	}

	Gui::~Gui()
	{
	}

	void Gui::removeWidget(const Widget::Ptr& widget, bool recursive)
	{
		mRoot->removeChild(widget, recursive);
	}

	GuiContainer::Ptr Gui::createContainer(const Widget::Ptr& parent, const std::string& name)
	{
		GuiContainer::Ptr widget(new GuiContainer(parent, this, name));
		parent->addChild(widget, name);
		return widget;
	}

	Panel::Ptr Gui::createPanel(const Widget::Ptr& parent, const std::string& name)
	{
		Panel::Ptr widget(new Panel(parent, this, name));
		parent->addChild(widget, name);
		return widget;
	}

	Label::Ptr Gui::createLabel(const Widget::Ptr& parent, const std::string& name, const std::string& text, sf::Font* font, Label::TextWrap wrap, const std::string& theme)
	{
		std::string t = theme == "" ? mThemeCache.defaultTheme : theme;
		Label::Ptr widget(new Label(parent, this, name, text, getTheme(t), font, wrap));
		parent->addChild(widget, name);
		return widget;
	}

	TextButton::Ptr Gui::createTextButton(const Widget::Ptr& parent, const std::string& name, const std::string& text, const std::string& theme)
	{
		std::string t = theme == "" ? mThemeCache.defaultTheme : theme;
		TextButton::Ptr widget(new TextButton(parent, this, name, text, getTheme(t)));
		parent->addChild(widget, name);
		return widget;
	}

	ImageButton::Ptr Gui::createImageButton(const Widget::Ptr& parent, const std::string& name, sf::Texture* texture, sf::IntRect& imageRect, const std::string& theme)
	{
		std::string t = theme == "" ? mThemeCache.defaultTheme : theme;
		ImageButton::Ptr widget(new ImageButton(parent, this, name, texture, imageRect, getTheme(t)));
		parent->addChild(widget, name);
		return widget;
	}

	// With standalone texture
	Image::Ptr Gui::createImage(const Widget::Ptr& parent, const std::string& name, sf::Texture* texture, sf::IntRect& rect)
	{
		Image::Ptr widget(new Image(parent, this, name, texture, rect));
		parent->addChild(widget, name);
		return widget;
	}

	ChildWindow::Ptr Gui::createChildWindow(const Widget::Ptr& parent, const std::string& name, const std::string& title, const std::string& theme)
	{
		std::string t = theme == "" ? mThemeCache.defaultTheme : theme;
		ChildWindow::Ptr widget(new ChildWindow(parent, this, name, title, getTheme(t)));
		parent->addChild(widget, name);
		return widget;
	}

	ScrollBar::Ptr Gui::createScrollBar(const Widget::Ptr& parent, const std::string& name, const std::string& theme)
	{
		std::string t = theme == "" ? mThemeCache.defaultTheme : theme;
		ScrollBar::Ptr widget(new ScrollBar(parent, this, name, getTheme(t)));
		parent->addChild(widget, name);
		return widget;
	}

	//convert from sf::Event to GuiEvent
	bool Gui::handleEvent(sf::Event& event)
	{
		GuiEvent guiEvent;
		Widget::Ptr focusedWidget = nullptr;
		std::vector<gui::Widget::Ptr>* children = &mRoot->mChildWidgets;

		switch (event.type)
		{
		case sf::Event::TextEntered:
			//send to focused
			guiEvent.type = GuiEvent::TextEntered;
			guiEvent.text = event.text;
			if (mFocusedWidget && mFocusedWidget->isEnabled())
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
			if (mFocusedWidget && mFocusedWidget->isEnabled())
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
			if (mFocusedWidget && mFocusedWidget->isEnabled())
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

			//check to see if the click was on UI and set focused widget accordingly
			for (auto& widget = children->begin(); widget != children->end(); )
			{
				if ((*widget)->isEnabled() && (*widget)->mouseOnWidget(guiEvent.mouseButton.x, guiEvent.mouseButton.y))
				{
					// Focus candidate found. Explore its children list now.
					focusedWidget = *widget;
					// Reset for loop
					children = &( (*widget)->mChildWidgets );
					widget = children->begin();
					continue;
				}
				++widget;
			}

			if (focusedWidget)
			{
				// found it, focus and pass event
				this->changeFocus(focusedWidget);
				focusedWidget->handleEvent(guiEvent);
				return true;
			}

			//click not on UI, unfocus and let event go
			this->noFocus();
			return false;

			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseButtonReleased:
			guiEvent.type = GuiEvent::MouseButtonReleased;
			guiEvent.mouseButton = event.mouseButton;

			// check if a mouse drag has just ended
			if (mFocusedWidget && mFocusedWidget->isEnabled() && mFocusedWidget->isDragging())
			{
				if (guiEvent.mouseButton.button == sf::Mouse::Button::Left)
				{
					GuiEvent dragEnd;
					dragEnd.type = GuiEvent::MouseDragEnd;
					dragEnd.mouseDrag.x = event.mouseButton.x;
					dragEnd.mouseDrag.y = event.mouseButton.y;
					mFocusedWidget->handleEvent(dragEnd);
					mFocusedWidget->setDragging(false);
				}
			}
			break;
			//////////////////////////////////////////////////////////////////////
		case sf::Event::MouseMoved:
			guiEvent.type = GuiEvent::MouseMoved;
			guiEvent.mouseMove = event.mouseMove;

			if (mFocusedWidget && mFocusedWidget->isEnabled() && mFocusedWidget->isDraggable())
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					// Drag detected
					sf::Vector2i newMousePos(event.mouseMove.x, event.mouseMove.y);
					sf::Vector2i deltaPos = newMousePos - getOldMousePosition();

					mFocusedWidget->move(sf::Vector2f(deltaPos));

					if (mFocusedWidget->isDragging())
					{
						// DragBegin already been called, continue generating drag events
						GuiEvent drag;
						drag.type = GuiEvent::MouseDrag;
						drag.mouseDrag = event.mouseMove;
						mFocusedWidget->handleEvent(drag);
					}
					else
					{
						// Generate DragBegin
						GuiEvent dragBegin;
						dragBegin.type = GuiEvent::MouseDragBegin;
						dragBegin.mouseDrag = event.mouseMove;
						mFocusedWidget->handleEvent(dragBegin);
						mFocusedWidget->setDragging(true);
					}
				}
			}

			// check to create mouseEntered and mouseLeft events
			mRoot->checkMouseEnterLeft(event.mouseMove.x, event.mouseMove.y);

			updateMousePos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
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
			if (widget->isEnabled() && widget->handleEvent(guiEvent)) return true;
		}

		return false;
	}

	void Gui::update()
	{
		for (auto& widget : mRoot->mChildWidgets)
		{
			widget->update();
		}
	}

	void Gui::setScissor(const sf::FloatRect& scissor) 
	{
		// Calculate intersection between the two rectangles
		sf::FloatRect resultRect = scissor;
		if (mGLScissorCoords.left > scissor.left)
		{
			resultRect.left = mGLScissorCoords.left;
		}
		if (mGLScissorCoords.top > scissor.top)
		{
			resultRect.top = mGLScissorCoords.top;
		}
		if (mGLScissorCoords.left + mGLScissorCoords.width < scissor.left + scissor.width)
		{
			resultRect.width = mGLScissorCoords.left + mGLScissorCoords.width - scissor.left;
		}
		if (mGLScissorCoords.top + mGLScissorCoords.height < scissor.top + scissor.height)
		{
			resultRect.height = mGLScissorCoords.top + mGLScissorCoords.height - scissor.top;
		}

		resultRect.width = resultRect.width > 0 ? resultRect.width : 0;
		resultRect.height = resultRect.height > 0 ? resultRect.height : 0;

		mGLScissorCoords = resultRect;
		glScissor(
			mGLScissorCoords.left,
			mWindow->getSize().y - mGLScissorCoords.top - mGLScissorCoords.height,
			mGLScissorCoords.width,
			mGLScissorCoords.height);

		dirtyScissor = true;
	}

	void Gui::resetScissor()
	{
		if (!dirtyScissor)
			return;

		sf::Vector2u size = mWindow->getSize();
		mGLScissorCoords = sf::FloatRect(0, 0, size.x, size.y);

		glScissor(mGLScissorCoords.left, mGLScissorCoords.top, mGLScissorCoords.width, mGLScissorCoords.height);

		dirtyScissor = false;
	}

	void Gui::draw()
	{
		resetScissor();

		glEnable(GL_SCISSOR_TEST);

		mWindow->draw(*mRoot);

		glDisable(GL_SCISSOR_TEST);
	}

	sf::Font* Gui::getFont(const std::string& name)
	{ 
		if (mFonts.find(name) != mFonts.end())
		{
			return &mFonts[name];
		}
		else return nullptr;
	}

	sf::Font* Gui::getDefaultFont()
	{
		return getFont(mDefaultFont);
	}

	void Gui::addFont(const std::string& name, sf::Font& font)
	{
		if (mFonts.find(name) != mFonts.end())
		{
			mFonts[name] = font;
		}
	}

	void Gui::addTheme(const std::string& filename)
	{
		if (!mThemeCache.loadTheme(filename))
		{
			std::cout << "Error loading theme " + filename << std::endl;
		}
	}

	void Gui::changeFocus(const Widget::Ptr& w)
	{
		assert(w != nullptr);

		if (w != mFocusedWidget)
		{
			noFocus();

			if (w->allowFocus())
			{
				mFocusedWidget = w;

				mFocusedWidget->focus();
			}
		}
	}

	void Gui::noFocus()
	{
		if (mFocusedWidget)
			mFocusedWidget->unfocus();

		mFocusedWidget = nullptr;
	}
}