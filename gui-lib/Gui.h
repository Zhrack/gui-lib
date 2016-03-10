#ifndef GUI_H
#define GUI_H


#include "Panel.h"
#include "GuiContainer.h"
#include "Label.h"
#include "Image.h"
#include "TextButton.h"
#include "ImageButton.h"
#include "ChildWindow.h"
#include "ScrollBar.h"

#include "Theme.h"

#include <vector>
#include <SFML\Window\Event.hpp>
#include <memory>
#include <string>

namespace gui
{
	//class Widget;
	//class Widget::Ptr;
	//class Panel;

	class Gui
	{
	public:
		// Constructors
		Gui();

		Gui(sf::RenderWindow* target);

		//////////////////////////////////////////////////
		// Destructor
		~Gui();

		// Convert from sf::Event to GuiEvent
		bool handleEvent(sf::Event& event);

		// Manage focus changes
		void changeFocus(const Widget::Ptr& w);

		void noFocus();

		// Getters and setters
		Widget::Ptr getFocusedWidget() const { return mFocusedWidget; }
		sf::RenderWindow* getWindow() const { return mWindow; }
		void setWindow(sf::RenderWindow* window) { mWindow = window; }
		const sf::Vector2i& getOldMousePosition() const { return mOldMousePos; }
		const GuiContainer::Ptr& getRoot() const { return mRoot; }

		void updateMousePos(const sf::Vector2i& pos){ mOldMousePos = pos; }

		// Remove widget
		void removeWidget(const Widget::Ptr& widget, bool recursive = false);

		// Get font by name
		sf::Font* getFont(const std::string& name);
		
		sf::Font* getDefaultFont();
		// Add new font
		void addFont(const std::string& name, sf::Font& font);

		Theme* getTheme(const std::string& theme) { return mThemeCache.getTheme(theme); }
		Theme* getDefaultTheme() { return mThemeCache.getTheme(mThemeCache.defaultTheme); }

		void addTheme(const std::string& filename);

		// glScissor management
		sf::FloatRect& getScissor() { return mGLScissorCoords; }
		void setScissor(const sf::FloatRect& scissor);

		void resetScissor();

		//////////////////////////////////////////////////
		///					WIDGET CREATION
		//////////////////////////////////////////////////
		//TODO implementare funzioni create man mano che vengono aggiunti widget

		GuiContainer::Ptr createContainer(const Widget::Ptr& parent, const std::string& name);
		GuiContainer::Ptr createContainer(const std::string& name){ return createContainer(mRoot, name); }

		Panel::Ptr createPanel(const Widget::Ptr& parent, const std::string& name);
		Panel::Ptr createPanel(const std::string& name){ return createPanel(mRoot, name); }

		Label::Ptr createLabel(const Widget::Ptr& parent, const std::string& name, const std::string& text = "", sf::Font* font = nullptr, Label::TextWrap wrap = Label::TextWrap::Free, const std::string& theme = "");
		Label::Ptr createLabel(const std::string& name, const std::string& text = "", sf::Font* font = nullptr, Label::TextWrap wrap = Label::TextWrap::Free, const std::string& theme = ""){ return createLabel(mRoot, name, text, font, wrap, theme); }

		TextButton::Ptr createTextButton(const Widget::Ptr& parent, const std::string& name, const std::string& text = "", const std::string& theme = "");
		TextButton::Ptr createTextButton(const std::string& name, const std::string& text, const std::string& theme = ""){ return createTextButton(mRoot, name, text, theme); }

		ImageButton::Ptr createImageButton(const Widget::Ptr& parent, const std::string& name, sf::Texture* texture, sf::IntRect& imageRect = sf::IntRect(), const std::string& theme = "");
		ImageButton::Ptr createImageButton(const std::string& name, sf::Texture* texture, sf::IntRect& imageRect = sf::IntRect(), const std::string& theme = "")
		{ return createImageButton(mRoot, name, texture, imageRect, theme); }

		Image::Ptr createImage(const Widget::Ptr& parent, const std::string& name, sf::Texture* texture, sf::IntRect& rect = sf::IntRect());
		Image::Ptr createImage(const std::string& name, sf::Texture* texture, sf::IntRect& rect = sf::IntRect()){ return createImage(mRoot, name, texture, rect); }

		ChildWindow::Ptr createChildWindow(const Widget::Ptr& parent, const std::string& name, const std::string& title, const std::string& theme = "");
		ChildWindow::Ptr createChildWindow(const std::string& name, const std::string& title, const std::string& theme = "")
		{
			return createChildWindow(mRoot, name, title, theme);
		}

		ScrollBar::Ptr createScrollBar(const Widget::Ptr& parent, const std::string& name, const std::string& theme = "");
		ScrollBar::Ptr createScrollBar(const std::string& name, const std::string& theme = "")
		{
			return createScrollBar(mRoot, name, theme);
		}

		void update();

		// Draw all widgets
		void draw();

	private:
		// glScissor coordinates
		sf::FloatRect mGLScissorCoords;
		// Used to reset the scissor
		bool dirtyScissor;

		// Currently focused widget, for keyboard events etc...
		Widget::Ptr mFocusedWidget;

		// Root of the widget tree. Every widget has this as parent if not specified
		// Its rect shape is as big as the screen
		GuiContainer::Ptr mRoot;

		sf::RenderWindow* mWindow;

		sf::Vector2i mOldMousePos;

		// GUI fonts
		std::unordered_map<std::string, sf::Font> mFonts;
		const std::string mDefaultFont;

		// Theme manager and loader
		ThemeCache mThemeCache;
	};
}

#endif