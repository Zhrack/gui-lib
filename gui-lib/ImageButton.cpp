#include "ImageButton.h"

namespace gui
{
	// ImageButton stores a pointer to the texture, so lifetime of the texture must be managed.
	ImageButton::ImageButton(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Vector2f* points, sf::Texture* tex) :
		BorderWidget(parent, gui, name, points),
		mImage(new Image(static_cast<Widget::Ptr>(this), gui, "", tex))
	{
	}


	ImageButton::~ImageButton()
	{
	}

	//	Points must be an array of size 4, with coords of 4 points in pixel space of the texture
	void ImageButton::setTexture(sf::Texture* tex, sf::Vector2f* points)
	{
		mImage->mRect.setTexture(tex);

		if (points)
		{
			updateNinePatchPoints(points);
		}
	}

	void ImageButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isEnabled() && isVisible())
		{
			target.draw(mRect, states); // debug
			target.draw(*mImage, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}
} // namespace