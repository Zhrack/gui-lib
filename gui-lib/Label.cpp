#include "Label.h"

#include "Gui.h"

namespace gui
{

	Label::Label(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Font* font, const std::string& theme) :
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

		if (theme == "")
		{
			Theme* t = mMainGui->getDefaultTheme();
			mText.setColor(t->label.textColor);
		}
		else
		{
			Theme* t = mMainGui->getTheme(theme);
			mText.setColor(t->label.textColor);
		}

		mRect.setFillColor(sf::Color::Transparent);
		updateTextTransform();
	}


	Label::~Label()
	{
	}

	void Label::setText(const std::string& str)
	{
		mText.setString(str);

		updateTextTransform();
	}

	void Label::updateTextTransform()
	{
		sf::FloatRect textRect = mText.getLocalBounds();
		
		mRect.setSize(sf::Vector2f(textRect.left + textRect.width, textRect.top + textRect.height));

		mText.setPosition(mRect.getPosition());
		mText.setRotation(mRect.getRotation());
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
		updateTextTransform();
	}
} // namespace