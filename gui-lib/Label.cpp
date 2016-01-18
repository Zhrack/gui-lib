#include "Label.h"

#include "Gui.h"
#include "Theme.h"
#include <iostream>
#include <sstream>
#include <assert.h>

namespace gui
{
	Label::Label(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& text, Theme* theme, sf::Font* font, TextWrap wrap) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(50, 50), true, true, false, true, false),
		mText(),
		mWrap(wrap)
	{
		if (font)
		{
			mText.setFont(*font);
		}
		else
		{
			mText.setFont(*(mMainGui->getDefaultFont()));
		}

		setText(text);
		mText.setColor(theme->label.textColor);
		setCharacterSize(theme->label.textSize);
	}


	Label::~Label()
	{
	}

	void Label::setText(const std::string& str)
	{
		if (TextWrap::Free == mWrap)
		{
			mText.setString(str);
		}
		else if (TextWrap::Horizontal == mWrap)
		{
			// Split string to have an array of tokens
			std::vector<std::string> strings = split(str, ' ');

			float rectWidth = mRect.getSize().x;
			std::string text = "";
			sf::Text tempText(mText);
			tempText.setString("");
			
			// Add the token until the line fits inside
			for (auto& iter = strings.begin(); iter != strings.end(); ++iter)
			{
				// Check to see if current text + iter fits inside parent width
				if (iter == strings.begin())
				{
					text = *iter;
				}
				else
				{
					text = " " + *iter;
				}

				tempText.setString(tempText.getString() + text);

				if (tempText.getLocalBounds().width > rectWidth)
				{
					// Must create a new line!
					sf::String temp = tempText.getString();
					// Remove last token and add it as new line
					text = temp.substring(0, temp.getSize() - text.length());
					text += "\n" + *iter;
					tempText.setString(text);
				}
			}

			mText.setString(tempText.getString());
		}

		setDirty();
		update();
	}

	void Label::setCharacterSize(unsigned int size)
	{
		mText.setCharacterSize(size);

		setDirty();
		update();
	}

	void Label::setWrapStyle(TextWrap wrap, int maxWidth, const std::string& text)
	{
		if (maxWidth <= 0 && wrap == TextWrap::Horizontal)
			return;

		mWrap = wrap;

		mRect.setSize(sf::Vector2f(maxWidth, mRect.getSize().y));

		if (text == "")
		{
			// Reuse the actual text
			setText(mText.getString());
		}
		else
			setText(text);
	}

	void Label::setGlobalPosition(const sf::Vector2f& globalPos)
	{
		mRect.setPosition(globalPos);
		
		setDirty();
		update();
	}

	void Label::setGlobalPosition(float x, float y)
	{
		setGlobalPosition(sf::Vector2f(x, y));
	}

	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isEnabled() && isVisible())
		{
			//target.draw(mRect, states); // debug
			target.draw(mText, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}

	std::vector<std::string>& Label::split(const std::string& s, char delim, std::vector<std::string>& elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	std::vector<std::string> Label::split(const std::string& s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	void Label::update()
	{
		if (isDirty())
		{
			sf::FloatRect textRect = mText.getLocalBounds();
			if (TextWrap::Free == mWrap)
			{
				mRect.setSize(sf::Vector2f(textRect.left + textRect.width, textRect.top + textRect.height));
			}
			else if (TextWrap::Horizontal == mWrap)
			{
				mRect.setSize(sf::Vector2f(mRect.getSize().x, textRect.top + textRect.height));
			}

			mText.setPosition(mRect.getPosition());
			mText.setRotation(mRect.getRotation());		

			setClean();
		}
	}
} // namespace