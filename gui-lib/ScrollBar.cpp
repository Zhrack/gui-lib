#include "ScrollBar.h"

#include "Theme.h"

namespace gui
{
	const std::string ScrollBar::mThumbName = "thumb";
	const std::string ScrollBar::mArrowUpRightName = "arrowUpRight";
	const std::string ScrollBar::mArrowDownLeftName = "arrowDownLeft";

	ScrollBar::ScrollBar(const Widget::Ptr& parent, Gui* const gui, const std::string& name, Theme* theme) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(100, 15), true, true, false, true, false),
		
		mValue(1),
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

		mThumb->setBorderRendered(false);
		mArrowUpRight->setBorderRendered(false);
		mArrowDownLeft->setBorderRendered(false);

		mRect.setTexture(theme->texture);
		mRect.setTextureRect(theme->scrollBar.backgroundRect);

		mThumb->getShape().setFillColor(sf::Color::Green);
		mArrowUpRight->getShape().setFillColor(sf::Color::Blue);
		mArrowDownLeft->getShape().setFillColor(sf::Color::Red);

		setSize(50, 300);
		
		setMaximumArea(100);
		setViewableArea(20);
		
		//hideArrows();
		setValue(100);
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

			resizeThumb(getSize());

			if (mMaximumArea < mViewableArea)
			{
				setValue(0);
			}
			else if (mValue > mMaximumArea - mViewableArea)
			{
				setValue(mMaximumArea - mViewableArea);
			}
		}
	}

	void ScrollBar::setMaximumArea(uint maximum)
	{
		if (mMaximumArea != maximum)
		{
			mMaximumArea = maximum;

			resizeThumb(getSize());

			if (mMaximumArea < mViewableArea)
			{
				setValue(0);
				if (mAutoHide)
					hide();
			}
			else if (mValue > mMaximumArea - mViewableArea)
			{
				
				setValue(mMaximumArea - mViewableArea);

				if (mAutoHide)
					show();
			}
				
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

			if (mMaximumArea < mViewableArea)
			{
				mValue = 0;
			}
			else if (mValue > mMaximumArea - mViewableArea)
			{
				mValue = mMaximumArea - mViewableArea;
			}

			int arrowSize = 0;
			if (mShowArrows)
			{
				arrowSize = mArrowDownLeft->getSize().y;
			}

			// Reposition thumb
			if (mVertical)
			{
				mThumb->setPosition(0, calculateThumbPos(arrowSize));
			}
			else // Horizontal
			{
				mThumb->setPosition(calculateThumbPos(arrowSize), 0);
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
			int sbSize = mRect.getSize().y  -  arrowSize * 2;
			mThumb->setSize(newSize.x, (sbSize * mViewableArea) / mMaximumArea);

			mThumb->setPosition(0, calculateThumbPos(arrowSize));
		}
		else // Horizontal
		{
			int sbSize = mRect.getSize().x - arrowSize * 2;
			mThumb->setSize((sbSize * mViewableArea) / mMaximumArea, newSize.y);

			mThumb->setPosition(calculateThumbPos(arrowSize), 0);
		}
	}

	int ScrollBar::calculateThumbPos(int arrowSize)
	{
		int sbSize = mVertical 
			? (mRect.getSize().y - arrowSize * 2) 
			: (mRect.getSize().x - arrowSize * 2);

		int pos = (sbSize * mValue) / mMaximumArea;
		if (pos < arrowSize)
		{
			pos += arrowSize;
		}

		return pos;
	}

	void ScrollBar::setSize(const sf::Vector2f& newSize)
	{
		mRect.setSize(newSize);

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

		resizeThumb(newSize);
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
			mThumb->setPosition(0, calculateThumbPos(arrowSize));

			mArrowUpRight->setPosition(0, 0);
			mArrowDownLeft->setPosition(0, mRect.getSize().y - mArrowDownLeft->getSize().x); // mRectsize.y - arrow size.x
		}
		else // Horizontal
		{
			mThumb->setPosition(calculateThumbPos(arrowSize), 0);

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

			

			target.draw(*mArrowUpRight, states);
			//states.transform.rotate(180);
			target.draw(*mArrowDownLeft, states);
			target.draw(*mThumb, states);

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