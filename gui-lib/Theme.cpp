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

				if (!removeWhitespace(line, c))
				{
					std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
					lineError = true;
				}

				// There has to be an assignment character
				if (*c == '=')
					++c;
				else
				{
					std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
					lineError = true;
				}

				if (!removeWhitespace(line, c))
				{
					std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
					lineError = true;
				}

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
							sf::FloatRect rect;
							if (readFloatRect(value, rect))
							{
								// Get first and fourth points of BorderWidget(the external ones)
								newTheme->textButton.normalState.externalMargin.left = rect.left;
								newTheme->textButton.normalState.externalMargin.top = rect.top;
								newTheme->textButton.normalState.externalMargin.width = rect.left + rect.width;
								newTheme->textButton.normalState.externalMargin.height = rect.top + rect.height;
							}
							else
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "externalMarginHover")
						{
							sf::FloatRect rect;
							if (readFloatRect(value, rect))
							{
								newTheme->textButton.hoverState.externalMargin.left = rect.left;
								newTheme->textButton.hoverState.externalMargin.top = rect.top;
								newTheme->textButton.hoverState.externalMargin.width = rect.left + rect.width;
								newTheme->textButton.hoverState.externalMargin.height = rect.top + rect.height;
							}
							else
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "externalMarginDown")
						{
							sf::FloatRect rect;
							if (readFloatRect(value, rect))
							{
								newTheme->textButton.downState.externalMargin.left = rect.left;
								newTheme->textButton.downState.externalMargin.top = rect.top;
								newTheme->textButton.downState.externalMargin.width = rect.left + rect.width;
								newTheme->textButton.downState.externalMargin.height = rect.top + rect.height;
							}
							else
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "internalMarginNormal")
						{
							sf::FloatRect rect;
							if (readFloatRect(value, rect))
							{
								// Get second and third points of BorderWidget(the internal ones)
								newTheme->textButton.normalState.internalMargin.left = rect.left;
								newTheme->textButton.normalState.internalMargin.top = rect.top;
								newTheme->textButton.normalState.internalMargin.width = rect.left + rect.width;
								newTheme->textButton.normalState.internalMargin.height = rect.top + rect.height;
							}
							else
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "internalMarginHover")
						{
							sf::FloatRect rect;
							if (readFloatRect(value, rect))
							{
								newTheme->textButton.hoverState.internalMargin.left = rect.left;
								newTheme->textButton.hoverState.internalMargin.top = rect.top;
								newTheme->textButton.hoverState.internalMargin.width = rect.left + rect.width;
								newTheme->textButton.hoverState.internalMargin.height = rect.top + rect.height;

							}
							else
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "internalMarginDown")
						{
							sf::FloatRect rect;
							if (readFloatRect(value, rect))
							{
								newTheme->textButton.downState.internalMargin.left = rect.left;
								newTheme->textButton.downState.internalMargin.top = rect.top;
								newTheme->textButton.downState.internalMargin.width = rect.left + rect.width;
								newTheme->textButton.downState.internalMargin.height = rect.top + rect.height;

							}
							else
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
								newTheme->textButton.label.textColor = color;
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
							sf::FloatRect rect;
							if (readFloatRect(value, rect))
							{
								newTheme->childWindow.margins.externalMargin.left = rect.left;
								newTheme->childWindow.margins.externalMargin.top = rect.top;
								newTheme->childWindow.margins.externalMargin.width = rect.left + rect.width;
								newTheme->childWindow.margins.externalMargin.height = rect.top + rect.height;
							}
							else
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "internalMargin")
						{
							sf::FloatRect rect;
							if (readFloatRect(value, rect))
							{
								newTheme->childWindow.margins.internalMargin.left = rect.left;
								newTheme->childWindow.margins.internalMargin.top = rect.top;
								newTheme->childWindow.margins.internalMargin.width = rect.left + rect.width;
								newTheme->childWindow.margins.internalMargin.height = rect.top + rect.height;

							}
							else
							{
								std::cout << "Error: Failed to parse line " + std::to_string(lineNumber) + "." << std::endl;
								lineError = true;
							}
						}
						else if (property == "closeButtonRect")
						{
							sf::IntRect rect;
							if (readIntRect(value, rect))
							{
								newTheme->childWindow.closeButtonRect.left = rect.left;
								newTheme->childWindow.closeButtonRect.top = rect.top;
								newTheme->childWindow.closeButtonRect.width = rect.width;
								newTheme->childWindow.closeButtonRect.height = rect.height;

							}
							else
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
			if ((*c != ' ') && (*c != '\t') && (*c != '\r'))
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