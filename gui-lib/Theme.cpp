#include "Theme.h"
#include <string>
#include <fstream>
#include <locale>
#include <iostream>

namespace gui
{
	ThemeCache::ThemeCache() :
		darkTheme("darkTheme"),
		themePath("themes/")
	{
		if (!loadTheme(darkTheme))
		{
			std::cout << "Error loading darkTheme" << std::endl;
		}
	}

	ThemeCache::~ThemeCache()
	{
		for (auto& it = mThemes.begin(); it != mThemes.end(); ++it)
		{
			delete it->second->texture;
			delete it->second;
			it->second = nullptr;
		}
	}

	Theme* ThemeCache::getTheme(const std::string& theme)
	{
		if (mThemes.find(theme) != mThemes.end())
		{
			return mThemes[theme];
		}
		else return nullptr;
	}

	// Load a new theme
	// filename: the name of the file to load and of the spritesheet, without file extensions
	bool ThemeCache::loadTheme(const std::string& filename)
	{
		// The file may be cached
		if (mThemes.find(filename) != mThemes.end())
			return true;

		std::ifstream file;

		// Open the file
		file.open((themePath + filename + ".conf").c_str(), std::ifstream::in);

		// Check if the file was opened
		if (!file.is_open())
			return false;

		Theme* newTheme(new Theme());
		sf::Texture* tex(new sf::Texture());
		if (!tex->loadFromFile(themePath + filename + ".png"))
		{
			std::cout << "Error: Creation of texture failed." << std::endl;
		}
		else
		{
			newTheme->texture = tex;
		}

		bool error = false;
		unsigned int lineNumber = 0;
		std::string sectionName;

		// Stop reading when we reach the end of the file
		while (!file.eof())
		{
			// Get the next line
			std::string line;
			std::getline(file, line);
			std::string::const_iterator c = line.begin();
			lineNumber++;

			// If the lines contains a '\r' at the end then remove it
			if(!line.empty() && line[line.size() - 1] == '\r')
				line.erase(line.size() - 1);

			// Skip empty lines
			if(line.empty() || !removeWhitespace(line, c))
				continue;

			if(!findSection(line, c, sectionName))
			{
				std::string property;
				std::string value;
				bool lineError = false;

				// Read the property in lowercase
				property = readWord(line, c);

				removeWhitespace(line, c);

				// There has to be an assignment character
				if (*c == '=')
					++c;
				else
				{
					std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
					lineError = true;
				}

				removeWhitespace(line, c);

				if (!lineError)
				{
					// Get value
					value = readWord(line, c);

					if (sectionName == "Label")
					{
						// value is a Color
						sf::Color color;
						if (readColor(value, color))
						{
							newTheme->label.textColor = color;
						}
						else
						{
							std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
							lineError = true;
						}
						
					}
					else if (sectionName == "Button")
					{
						if (property == "externalMarginNormal")
						{
							if (!getMargin(value, newTheme->button.normalState.externalMargin))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "externalMarginHover")
						{
							if (!getMargin(value, newTheme->button.hoverState.externalMargin))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "externalMarginDown")
						{
							if (!getMargin(value, newTheme->button.downState.externalMargin))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "internalMarginNormal")
						{
							if (!getMargin(value, newTheme->button.normalState.internalMargin))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "internalMarginHover")
						{
							if (!getMargin(value, newTheme->button.hoverState.internalMargin))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "internalMarginDown")
						{
							if (!getMargin(value, newTheme->button.downState.internalMargin))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "reactive")
						{
							if (!getBool(value, newTheme->button.reactive))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "textColor")
						{
							// value is a Color
							sf::Color color;
							if (readColor(value, color))
							{
								newTheme->button.label.textColor = color;
							}
							else
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
					}
					else if (sectionName == "ChildWindow")
					{
						if (property == "externalMargin")
						{
							if (!getMargin(value, newTheme->childWindow.margins.externalMargin))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "internalMargin")
						{
							if (!getMargin(value, newTheme->childWindow.margins.internalMargin))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "closeButtonRect")
						{
							if (!getTexRect(value, newTheme->childWindow.closeButtonRect))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "titleBarRect")
						{
							if (!getTexRect(value, newTheme->childWindow.titleBarRect))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "bodyRect")
						{
							if (!getTexRect(value, newTheme->childWindow.bodyRect))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "reactive")
						{
							if (!getBool(value, newTheme->childWindow.reactive))
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "textColor")
						{
							// value is a Color
							sf::Color color;
							if (readColor(value, color))
							{
								newTheme->childWindow.label.textColor = color;
							}
							else
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
					}
				}

				error |= lineError;
			}
		}

		if (!error)
		{
			mThemes[filename] = newTheme;
		}
		else
		{
			delete newTheme->texture;
			delete newTheme;
		}

		return !error;
	}

	bool ThemeCache::removeWhitespace(const std::string& line, std::string::const_iterator& c) const
	{
		while (c != line.end())
		{
			if ((*c == ' ') || (*c == '\t') || (*c == '\r'))
				++c;
			else
				return true;
		}

		return false;
	}

	bool ThemeCache::findSection(const std::string& line, std::string::const_iterator c, std::string& sectionName)
	{
		std::string str;
		if (line.empty() == false)
		{
			if (line[0] == '[' && line[line.length() - 1] == ']')
			{
				// Remove square brackets
				str = line.substr(1, line.length() - 2);
				sectionName = readWord(str, str.begin());

				return true;
			}
		}

		return false;
	}

	std::string ThemeCache::readWord(const std::string& line, std::string::const_iterator& c) const
	{
		std::string word;
		while (c != line.end())
		{
			if ((*c != ' ') && (*c != '\t') && (*c != '\r') && (*c != '='))
			{
				word.push_back(*c);
				++c;
			}
			else
			{
				return word;
			}
		}
		return word;
	}

	bool ThemeCache::readColor(const std::string& value, sf::Color& color) const
	{
		std::string str;

		// value is in the form "(r,g,b)"
		int a = 255;
		if (value.empty() == false)
		{
			if (value[0] == '(' && value[value.length() - 1] == ')')
			{
				// Remove brackets
				str = value.substr(1, value.length() - 1);

				// Search for forst comma
				auto commaPos = str.find(',');
				if (commaPos != std::string::npos)
				{
					// Get first value
					color.r = atoi(str.substr(0, commaPos).c_str());
					str.erase(0, commaPos + 1);

					commaPos = str.find(',');
					if (commaPos != std::string::npos)
					{
						// Get second value
						color.g = atoi(str.substr(0, commaPos).c_str());
						str.erase(0, commaPos + 1);

						// Get third value
						color.b = atoi(str.c_str());
						color.a = a;

						return true;
					}
				}
			}
		}

		return false;
	}

	bool ThemeCache::readFloatRect(std::string value, sf::FloatRect& rect) const
	{
		std::string str;
		// Make sure that the line isn't empty
		if (value.empty() == false)
		{
			// The first and last character have to be brackets
			if ((value[0] == '(') && (value[value.length() - 1] == ')'))
			{
				// Remove brackets
				str = value.substr(1, value.length() - 1);

				// Search for the first comma
				auto commaPos = str.find(',');
				if (commaPos != std::string::npos)
				{
					// Get the left value and delete this part of the string
					rect.left = atof(str.substr(0, commaPos).c_str());
					str.erase(0, commaPos + 1);

					// Search for the second comma
					commaPos = str.find(',');
					if (commaPos != std::string::npos)
					{
						// Get the top value and delete this part of the string
						rect.top = atof(str.substr(0, commaPos).c_str());
						str.erase(0, commaPos + 1);

						// Search for the third comma
						commaPos = str.find(',');
						if (commaPos != std::string::npos)
						{
							// Get the width value and delete this part of the string
							rect.width = atof(str.substr(0, commaPos).c_str());
							str.erase(0, commaPos + 1);

							// Get the height value
							rect.height = atof(str.c_str());

							return true;
						}
					}
				}
			}
		}

		return false;
	}

	bool ThemeCache::readIntRect(std::string value, sf::IntRect& rect) const
	{
		sf::FloatRect fRect;
		if (readFloatRect(value, fRect))
		{
			rect.left = fRect.left;
			rect.top = fRect.top;
			rect.width = fRect.width;
			rect.height = fRect.height;
			return true;
		}
		else
			return false;
	}

	bool ThemeCache::getMargin(const std::string& value, sf::FloatRect& rect)
	{
		sf::FloatRect temp;
		if (readFloatRect(value, temp))
		{
			rect.left = temp.left;
			rect.top = temp.top;
			rect.width = temp.left + temp.width;
			rect.height = temp.top + temp.height;
			return true;
		}
		else
		{
			std::cout << "Parse error on margin." << std::endl;
			return false;
		}
	}

	bool ThemeCache::getTexRect(const std::string& value, sf::IntRect& rect)
	{
		sf::IntRect temp;
		if (readIntRect(value, temp))
		{
			rect.left = temp.left;
			rect.top = temp.top;
			rect.width = temp.width;
			rect.height = temp.height;
			return true;
		}
		else
		{
			std::cout << "Parse error on margin." << std::endl;
			return false;
		}
	}

	bool ThemeCache::getBool(const std::string& value, bool& b)
	{
		if (value == "true" || value == "TRUE" || value == "1")
		{
			b = true;
			return true;
		}
		else if (value == "false" || value == "FALSE" || value == "0")
		{
			b = false;
			return true;
		}
		else
		{
			std::cout << "Parse error on boolean: " << value << std::endl;
			return false;
		}
	}

	std::string ThemeCache::toLower(const std::string& str)
	{
		std::string result = "";
		std::locale loc;
		for (int i = 0; i < str.length(); ++i)
		{
			result.push_back(std::tolower(str[i], loc));
		}

		return result;
	}
}