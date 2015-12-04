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
	};

	// Holds theme data for textbuttons
	struct TextButtonData
	{
		//sf::FloatRect texRect;
		//sf::FloatRect internalMargins;
		TextureMarginsCoords normalState;
		TextureMarginsCoords hoverState;
		TextureMarginsCoords downState;
		LabelData label;
	};

	struct Theme
	{
		LabelData label;
		TextButtonData textButton;

		sf::Texture* texture;
	};

	class ThemeCache
	{
	public:
		class Gui;

		const std::string darkTheme;
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

		// Extract a sf::IntRect
		bool readFloatRect(std::string value, sf::FloatRect& rect) const;

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