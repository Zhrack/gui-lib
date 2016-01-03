#include "ImageButton.h"

#include "Theme.h"

namespace gui
{
	const std::string ImageButton::mImageName = "image";

	// ImageButton stores a pointer to the texture, so lifetime of the texture must be managed.
	ImageButton::ImageButton(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Texture* texture, sf::IntRect& imageRect, Theme* theme) :
		BorderWidget(parent, gui, name, theme, theme->button.reactive),
		mImage(new Image(static_cast<Widget::Ptr>(this), gui, mImageName, texture, imageRect))
	{
		setTexture(texture, imageRect);

		setBorderRendered(theme->button.renderBorder);
		setSize(mRect.getSize());
		setDirty();
	}


	ImageButton::~ImageButton()
	{
	}

	//	Points must be an array of size 4, with coords of 4 points in pixel space of the texture
	void ImageButton::setTexture(sf::Texture* tex, const sf::IntRect& clipRect)
	{
		mImage->setTexture(tex);
		setTextureRect(clipRect);

		//updateNinePatchPoints(rect1, rect2);
	}

	void ImageButton::setTextureRect(const sf::IntRect& rect)
	{
		mImage->setTextureRect(rect);
	}

	void ImageButton::setSize(const sf::Vector2f& newSize)
	{
		mImage->setSize(newSize);

		// now resize button accordingly
		resizeButton(newSize);

		setDirty();
	}

	void ImageButton::setSize(float x, float y)
	{
		setSize(sf::Vector2f(x, y));
	}

	void ImageButton::setPosition(const sf::Vector2f& localPos)
	{
		mRect.setPosition(localPos + mParent->getGlobalPosition());
		updateVertsPosition();

		sf::Vector2f marginOffset;
		if (mBorderRendered)
		{
			marginOffset = sf::Vector2f(mInternalMargins.left, mInternalMargins.top);
		}

		mImage->setPosition(marginOffset);
	}

	void ImageButton::setPosition(float x, float y)
	{
		setPosition(sf::Vector2f(x, y));
	}

	void ImageButton::setGlobalPosition(const sf::Vector2f& globalPos)
	{
		mRect.setPosition(globalPos);
		updateVertsPosition();

		sf::Vector2f marginOffset;
		if (mBorderRendered)
		{
			marginOffset = sf::Vector2f(mInternalMargins.left, mInternalMargins.top);
		}

		mImage->setPosition(marginOffset);
	}

	void ImageButton::setGlobalPosition(float x, float y)
	{
		setGlobalPosition(sf::Vector2f(x, y));
	}

	void ImageButton::update()
	{
		if (isDirty())
		{
			updateVertsPosition();

			sf::Vector2f marginOffset;
			if (mBorderRendered)
			{
				marginOffset = sf::Vector2f(mInternalMargins.left, mInternalMargins.top);
			}

			mImage->setPosition(marginOffset);

			setClean();
		}
	}

	void ImageButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isEnabled() && isVisible())
		{
			//target.draw(mRect, states); // debug
			if (mBorderRendered)
			{
				states.texture = mTexture;
				target.draw(mVerts, states);
				states = sf::RenderStates::Default;
			}
			target.draw(*mImage, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}
} // namespace