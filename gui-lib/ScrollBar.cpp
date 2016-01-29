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

		addChild(mThumb, mThumbName);
		addChild(mArrowUpRight, mArrowUpRightName);
		addChild(mArrowDownLeft, mArrowDownLeftName);

		mRect.setTexture(theme->texture);
		mRect.setTextureRect(theme->scrollBar.backgroundRect);

		mThumb->getShape().setFillColor(sf::Color::Green);
		mArrowUpRight->getShape().setFillColor(sf::Color::Blue);
		mArrowDownLeft->getShape().setFillColor(sf::Color::Red);

		sf::Vector2f origin(mArrowDownLeft->getSize().x / 2, mArrowDownLeft->getSize().y / 2);
		mArrowDownLeft->setOrigin(origin);
		mArrowDownLeft->setRotation(180);

		setSize(50, 300);
		
		setMaximumArea(100);
		setViewableArea(50);
		
		setValue(0);
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
		}
	}

	void ScrollBar::setViewableArea(uint viewableArea)
	{
		if (mViewableArea != viewableArea)
		{
			mViewableArea = viewableArea;

			if (mViewableArea < 10)
				mViewableArea = 10;

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

		resizeThumb(getSize());

		setDirty();
		update();
	}

	void ScrollBar::hideArrows()
	{
		mArrowUpRight->disable();
		mArrowDownLeft->disable();

		mShowArrows = false;

		resizeThumb(getSize());

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
		if (mShowArrows || pos < arrowSize)
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
			mArrowDownLeft->setPosition(newSize.x - newSize.x, 0); // mRectsize.x - arrow size.y
		}

		resizeThumb(newSize);
	}

	void ScrollBar::setSize(float x, float y)
	{
		setSize(sf::Vector2f(x, y));
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
			mArrowDownLeft->setPosition(0, mRect.getSize().y - arrowSize); // mRectsize.y - arrow size.x
		}
		else // Horizontal
		{
			mThumb->setPosition(calculateThumbPos(arrowSize), 0);

			mArrowUpRight->setPosition(0, 0);
			mArrowDownLeft->setPosition(mRect.getSize().x - arrowSize, 0); // mRectsize.x - arrow size.y
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
			target.draw(*mArrowDownLeft, states);

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