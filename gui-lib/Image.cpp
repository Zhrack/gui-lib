#include "Image.h"

namespace gui
{
	Image::Image(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Texture* tex) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(50, 50), true, true, false, true, false)
	{
		mRect.setFillColor(sf::Color::White);
		mRect.setTexture(tex);
	}


	Image::~Image()
	{
	}

	void Image::setTexture(sf::Texture* tex, bool resetRect)
	{
		mRect.setTexture(tex, resetRect);
	}

	void Image::setTextureRect(sf::IntRect& rect)
	{
		mRect.setTextureRect(rect);
	}

	void Image::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isEnabled() && isVisible())
		{
			target.draw(mRect, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}
} // namespace