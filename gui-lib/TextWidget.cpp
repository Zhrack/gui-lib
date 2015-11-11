#include "TextWidget.h"

#include "Gui.h"

namespace gui
{

	TextWidget::TextWidget(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Font* font) :
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

		mRect.setFillColor(sf::Color::Green);
		updateTextTransform();
	}


	TextWidget::~TextWidget()
	{
	}

	void TextWidget::setText(const std::string& str)
	{
		mText.setString(str);

		updateTextTransform();
	}

	void TextWidget::updateTextTransform()
	{
		sf::FloatRect textRect = mText.getLocalBounds();
		
		mRect.setSize(sf::Vector2f(textRect.left + textRect.width, textRect.top + textRect.height));

		mText.setPosition(mRect.getPosition());
		mText.setRotation(mRect.getRotation());
	}

	void TextWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
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

	void TextWidget::update()
	{
		updateTextTransform();
	}
} // namespace