#include "Image.h"
#include "Theme.h"

namespace gui
{
	Image::Image(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Texture* tex, sf::IntRect& rect) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(100, 100), true, true, false, true, false)
	{
		mRect.setFillColor(sf::Color::White);
		setTexture(tex);
		setTextureRect(rect);
	}


	Image::~Image()
	{
	}

	void Image::setTexture(sf::Texture* tex, bool resetRect)
	{
		mRect.setTexture(tex, resetRect);
	}

	void Image::setTextureRect(const sf::IntRect& rect)
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