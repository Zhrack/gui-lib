#ifndef GUITHEME_H
#define GUITHEME_H

#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\Texture.hpp>

#include <unordered_map>

#include "TextureMarginCoords.h"

namespace gui
{
	// Holds theme data for labels
	struct LabelData
	{
		sf::Color textColor;
		unsigned int textSize;
	};

	// Holds theme data for textbuttons and imagebuttons
	struct ButtonData
	{
		TextureMarginsCoords normalState;
		TextureMarginsCoords hoverState;
		TextureMarginsCoords downState;
		bool reactive;
		bool renderBorder;
		LabelData label;
	};

	// Holds theme data for childwindows
	struct ChildWindowData
	{
		TextureMarginsCoords margins;
		sf::IntRect closeButtonRect;
		bool closeButtonBorderRendered;
		sf::IntRect titleBarRect;
		sf::IntRect bodyRect;
		bool reactive;
		bool renderBorder;
		LabelData label;
	};

	struct Theme
	{
		LabelData label;
		ButtonData button;
		ChildWindowData childWindow;

		sf::Texture* texture;
	};

	class ThemeCache
	{
	public:
		class Gui;

		const std::string defaultTheme;
		const std::string themePath;

		ThemeCache();
		~ThemeCache();

		Theme* getTheme(const std::string& theme);

		// Load a new theme
		bool loadTheme(const std::string& filename);

	private:
		bool removeWhitespace(const std::string& line, std::string::const_iterator& c) const;

		bool findSection(const std::string& line, std::string::const_iterator c, std::string& sectionName);

		// Extract a color from a string
		bool readColor(const std::string& value, sf::Color& color) const;

		// Extract a sf::FloatRect
		bool readFloatRect(std::string value, sf::FloatRect& rect) const;
		// Extract a sf::IntRect
		bool readIntRect(std::string value, sf::IntRect& rect) const;
		// Extract margins for Border Widgets
		bool getMargin(const std::string& value, sf::FloatRect& rect);
		// Extract texture rects
		bool getTexRect(const std::string& value, sf::IntRect& rect);
		// Extract a boolean value
		bool getBool(const std::string& value, bool& b);
		// Extract a integer value
		unsigned int getUInt(const std::string& value);

		// Reads a new word while advancing the iterator
		std::string readWord(const std::string& line, std::string::const_iterator& iter) const;

		std::string toLower(const std::string& str);

	private:
		Gui* mGui;

		// Wigdet Themes
		std::unordered_map<std::string, Theme*> mThemes;
	};
}
#endif