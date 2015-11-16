#ifndef GUI_H
#define GUI_H


#include "Panel.h"
#include "GuiContainer.h"
#include "Label.h"
#include "Image.h"
#include "TextButton.h"
#include "ImageButton.h"

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
		Theme* getDefaultTheme() { return mThemeCache.getTheme(mThemeCache.darkTheme); }

		void addTheme(const std::string& filename);

		//////////////////////////////////////////////////
		///					WIDGET CREATION
		//////////////////////////////////////////////////
		//TODO implementare funzioni create man mano che vengono aggiunti widget

		Panel::Ptr createPanel(const Widget::Ptr& parent, const std::string& name);
		Panel::Ptr createPanel(const std::string& name){ return createPanel(mRoot, name); }

		Label::Ptr createLabel(const Widget::Ptr& parent, const std::string& name, sf::Font* font = nullptr, const std::string& theme = "");
		Label::Ptr createLabel(const std::string& name, sf::Font* font = nullptr, const std::string& theme = ""){ return createLabel(mRoot, name, font, theme); }

		//TextButton::Ptr createTextButton(const Widget::Ptr& parent, const std::string& name, const std::string& text = "");
		//TextButton::Ptr createTextButton(const std::string& name, const std::string& text){ return createTextButton(mRoot, name, text); }

		//ImageButton::Ptr createImageButton(const Widget::Ptr& parent, const std::string& name, const std::string& text = "");
		//ImageButton::Ptr createImageButton(const std::string& name, const std::string& text){ return createImageButton(mRoot, name, text); }

		//Image::Ptr createImage(const Widget::Ptr& parent, const std::string& name);
		//Image::Ptr createImage(const std::string& name){ return createImage(mRoot, name); }

		// Draw all widgets
		void draw() const;

	private:
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