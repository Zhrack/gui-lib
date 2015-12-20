#include "Label.h"

#include "Gui.h"
#include "Theme.h"
#include <iostream>

namespace gui
{
	Label::Label(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& text, Theme* theme, sf::Font* font) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(50, 50), true, true, false, true, false),
		mText()
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

		mRect.setFillColor(sf::Color::Transparent);
	}


	Label::~Label()
	{
	}

	void Label::setText(const std::string& str)
	{
		mText.setString(str);

		updateTextTransform();
	}

	void Label::setCharacterSize(unsigned int size)
	{
		mText.setCharacterSize(size);

		updateTextTransform();
	}

	void Label::updateTextTransform()
	{
		sf::FloatRect textRect = mText.getLocalBounds();
		
		mRect.setSize(sf::Vector2f(textRect.left + textRect.width, textRect.top + textRect.height));

		mText.setPosition(mRect.getPosition());
		mText.setRotation(mRect.getRotation());
	}

	void Label::setPosition(const sf::Vector2f& localPos)
	{
		mRect.setPosition(localPos + mParent->getGlobalPosition());

		updateTextTransform();
	}

	void Label::setPosition(float x, float y)
	{
		setPosition(sf::Vector2f(x, y));
	}

	void Label::setGlobalPosition(const sf::Vector2f& globalPos)
	{
		mRect.setPosition(globalPos);
		
		updateTextTransform();
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
			target.draw(mRect, states); // debug
			target.draw(mText, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}

	void Label::update()
	{
		if (isDirty())
		{
			updateTextTransform();
			setClean();
		}
	}
} // namespace