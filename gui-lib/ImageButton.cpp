#include "ImageButton.h"

namespace gui
{
	// ImageButton stores a pointer to the texture, so lifetime of the texture must be managed.
	ImageButton::ImageButton(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Texture* texture, sf::IntRect& imageRect, Theme* theme, bool reactive) :
		BorderWidget(parent, gui, name, theme, reactive),
		mImage(new Image(static_cast<Widget::Ptr>(this), gui, "", texture, imageRect))
	{
		setTexture(texture, imageRect);
	}


	ImageButton::~ImageButton()
	{
	}

	//	Points must be an array of size 4, with coords of 4 points in pixel space of the texture
	void ImageButton::setTexture(sf::Texture* tex, const sf::IntRect& clipRect)
	{
		mImage->setTexture(tex);

		//updateNinePatchPoints(rect1, rect2);
	}

	void ImageButton::setTextureRect(const sf::IntRect& rect)
	{
		mImage->setTextureRect(rect);
	}

	void ImageButton::resize(sf::Vector2f newSize)
	{
		mImage->resize(newSize);

		// now resize button accondingly
		resizeButton(newSize);
	}

	void ImageButton::resize(float x, float y)
	{
		resize(sf::Vector2f(x, y));
	}

	void ImageButton::update()
	{
		if (isDirty())
		{
			updateVertsPosition();

			mImage->setGlobalPosition(sf::Vector2f(
				getGlobalPosition().x + mInternalMargins.left,
				getGlobalPosition().y + mInternalMargins.top)
				);

			setClean();
		}
	}

	void ImageButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isEnabled() && isVisible())
		{
			target.draw(mRect, states); // debug
			states.texture = mTexture;
			target.draw(mVerts, states);
			states = sf::RenderStates::Default;
			target.draw(*mImage, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}
} // namespace