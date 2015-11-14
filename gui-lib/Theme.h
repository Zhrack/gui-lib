#ifndef GUITHEME_H
#define GUITHEME_H

#include <SFML\Graphics\Color.hpp>

#include "Gui.h"

namespace gui
{
	// Holds theme data for labels
	struct LabelData
	{
		sf::Color textColor;
	};

	struct Theme
	{
		LabelData label;
	};

	class ThemeCache
	{
	public:
		ThemeCache();
		~ThemeCache();

		Theme* getTheme(const std::string& theme);

		// Load a new theme
		bool loadTheme(const std::string& filename, Theme& result);

	private:
		bool removeWhitespace(const std::string& line, std::string::const_iterator& c) const;

		bool findSection(const std::string& line, std::string::const_iterator c, std::string& sectionName);

		// Extract a color from a string
		bool readColor(const std::string& value, sf::Color& color) const;

		// Extract a sf::IntRect
		bool readIntRect(std::string value, sf::IntRect& rect) const;

		// Reads a new word while advancing the iterator
		std::string readWord(const std::string& line, std::string::const_iterator& c) const;

		std::string toLower(const std::string& str);

	private:
		Gui* mGui;

		// Wigdet Themes
		std::unordered_map<std::string, Theme*> mThemes;
	};
}
#endif