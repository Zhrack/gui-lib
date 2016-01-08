#include "ScrollBar.h"

#include "Theme.h"

namespace gui
{
	const std::string ScrollBar::mThumbName = "thumb";
	const std::string ScrollBar::mArrowUpRightName = "arrowUpRight";
	const std::string ScrollBar::mArrowDownLeftName = "arrowDownLeft";

	ScrollBar::ScrollBar(const Widget::Ptr& parent, Gui* const gui, const std::string& name, Theme* theme) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(100, 15), true, true, false, true, false),
		
		mValue(0),
		mViewableArea(5),
		mMaximumArea(10),
		mScrollAmount(1),

		mAutoHide(true),
		mVertical(true),
		mShowArrows(true),

		mScrollableWidget(nullptr),
		mThumb(new ImageButton(static_cast<Widget::Ptr>(this), gui, mThumbName, theme->texture, theme->scrollBar.thumbRect, theme)),
		mArrowUpRight(new ImageButton(static_cast<Widget::Ptr>(this), gui, mArrowUpRightName, theme->texture, theme->scrollBar.arrowRect, theme)),
		mArrowDownLeft(new ImageButton(static_cast<Widget::Ptr>(this), gui, mArrowDownLeftName, theme->texture, theme->scrollBar.arrowRect, theme))
	{
		mThumb->setReactive(theme->scrollBar.reactiveThumb);
		mArrowUpRight->setReactive(theme->scrollBar.reactiveArrows);
		mArrowDownLeft->setReactive(theme->scrollBar.reactiveArrows);

		mRect.setTexture(theme->texture);
		mRect.setTextureRect(theme->scrollBar.backgroundRect);
	}


	ScrollBar::~ScrollBar()
	{
	}

	// Binds a widget to the scrollbar, its children will be scrolled
	void ScrollBar::bindScrollableWidget(const Widget::Ptr& widget, uint viewableArea, uint maximumArea)
	{
		mScrollableWidget = widget;

		setViewableArea(viewableArea);

		setMaximumArea(maximumArea);

		setValue(0);

		setDirty();
		update();
	}


	// Sets the scrollbar layout
	void ScrollBar::setVertical(bool vertical)
	{
		if (mVertical != vertical)
		{
			mVertical = vertical;

			// Swap dimensions
			sf::Vector2f size = getSize();
			setSize(size.y, size.x);
			if (mVertical)
			{
				setPosition(mParent->getSize().x - size.x, 0);
			}
			else
			{
				setPosition(mParent->getSize().y - size.y, 0);
			}
		}
	}

	void ScrollBar::setViewableArea(uint viewableArea)
	{
		if (mViewableArea != viewableArea)
		{
			mViewableArea = viewableArea;

			if (mViewableArea > mMaximumArea)
			{
				mViewableArea = mMaximumArea;
			}
		}
	}

	void ScrollBar::setMaximumArea(uint maximum)
	{
		if (mMaximumArea != maximum)
		{
			mMaximumArea = maximum;

			if (mMaximumArea < mViewableArea)
			{
				if (mAutoHide)
					hide();
			}
			else if (mAutoHide)
				show();
		}
	}


	// Sets the amount of scroll of the arrows, when pressed
	void ScrollBar::setScrollAmount(uint amount)
	{
		mScrollAmount = amount;			
	}


	// Decides whether to show arrows
	void ScrollBar::showArrows()
	{
		mArrowUpRight->enable();
		mArrowDownLeft->enable();

		mShowArrows = true;

		setDirty();
		update();
	}

	void ScrollBar::hideArrows()
	{
		mArrowUpRight->disable();
		mArrowDownLeft->disable();

		mShowArrows = false;

		setDirty();
		update();
	}


	// Sets the current value, clamped to [mViewableArea, mMaximumArea] or 0
	void ScrollBar::setValue(uint value)
	{
		if (mValue != value)
		{
			mValue = value;

			if (mValue < 0)
			{
				mValue = 0;
			}
			else if (mValue > mMaximumArea)
			{
				mValue = mMaximumArea;
			}

			int arrowSize = 0;
			if (mShowArrows)
			{
				arrowSize = mArrowDownLeft->getSize().y;
			}

			// Reposition thumb
			if (mVertical)
			{
				mThumb->setPosition(0, (mThumb->getSize().y * mValue) / mMaximumArea + arrowSize);
			}
			else // Horizontal
			{
				mThumb->setPosition((mThumb->getSize().x * mValue + arrowSize) / mMaximumArea, 0);
			}
		}
	}

	void ScrollBar::resizeThumb(const sf::Vector2f& newSize)
	{
		int arrowSize = 0;
		if (mShowArrows)
		{
			arrowSize = mArrowDownLeft->getSize().y;
		}

		if (mVertical)
		{
			mThumb->setSize(newSize.x, (mRect.getSize().y * mViewableArea) / mMaximumArea);

			mThumb->setPosition(0, (mThumb->getSize().y * mValue) / mMaximumArea + arrowSize);
		}
		else // Horizontal
		{
			mThumb->setSize((mRect.getSize().x * mViewableArea) / mMaximumArea, newSize.y);

			mThumb->setPosition((mThumb->getSize().x * mValue + arrowSize) / mMaximumArea, 0);
		}
	}

	void ScrollBar::setSize(const sf::Vector2f& newSize)
	{
		mRect.setSize(newSize);

		resizeThumb(newSize);

		if (mVertical)
		{
			mArrowUpRight->setSize(newSize.x, newSize.x);
			mArrowDownLeft->setSize(newSize.x, newSize.x);

			mArrowUpRight->setPosition(0, 0);
			mArrowDownLeft->setPosition(0, newSize.y - newSize.x); // mRectsize.y - arrow size.x
		}
		else // Horizontal
		{
			mArrowUpRight->setSize(newSize.y, newSize.y);
			mArrowDownLeft->setSize(newSize.y, newSize.y);

			mArrowUpRight->setPosition(0, 0);
			mArrowDownLeft->setPosition(newSize.x - newSize.y, 0); // mRectsize.x - arrow size.y
		}
	}

	void ScrollBar::setSize(float x, float y)
	{
		setSize(sf::Vector2f(x, y));
	}

	void ScrollBar::setPosition(const sf::Vector2f& localPos)
	{
		setGlobalPosition(localPos + mParent->getGlobalPosition());
	}

	void ScrollBar::setPosition(float x, float y)
	{
		setPosition(sf::Vector2f(x, y));
	}

	void ScrollBar::setGlobalPosition(const sf::Vector2f& globalPos)
	{
		mRect.setPosition(globalPos);

		int arrowSize = 0;
		if (mShowArrows)
		{
			arrowSize = mArrowDownLeft->getSize().y;
		}

		if (mVertical)
		{
			mThumb->setPosition(0, (mThumb->getSize().y * mValue) / mMaximumArea + arrowSize);

			mArrowUpRight->setPosition(0, 0);
			mArrowDownLeft->setPosition(0, mRect.getSize().y - mArrowDownLeft->getSize().x); // mRectsize.y - arrow size.x
		}
		else // Horizontal
		{
			mThumb->setPosition((mThumb->getSize().x * mValue + arrowSize) / mMaximumArea, 0);

			mArrowUpRight->setPosition(0, 0);
			mArrowDownLeft->setPosition(mRect.getSize().x - mArrowDownLeft->getSize().y, 0); // mRectsize.x - arrow size.y
		}
	}

	void ScrollBar::setGlobalPosition(float x, float y)
	{
		setGlobalPosition(sf::Vector2f(x, y));
	}


	void ScrollBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isEnabled() && isVisible())
		{
			target.draw(mRect, states);
			target.draw(*mThumb, states);
			target.draw(*mArrowUpRight, states);
			states.transform.scale(0, -1);
			target.draw(*mArrowDownLeft, states);
			states = sf::RenderStates::Default;

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}

	void ScrollBar::update()
	{

	}

} // namespace