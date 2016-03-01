#include "ScrollBar.h"

#include "Theme.h"

#include <iostream>

#define USEDEBUG
#ifdef USEDEBUG
#define Debug(x) std::cout << x
#else
#define Debug(x) 
#endif 

namespace gui
{	
	const std::string ScrollBar::mThumbName = "thumb";
	const std::string ScrollBar::mArrowUpLeftName = "arrowUpRight";
	const std::string ScrollBar::mArrowDownRightName = "arrowDownLeft";

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
		mArrowUpLeft(new ImageButton(static_cast<Widget::Ptr>(this), gui, mArrowUpLeftName, theme->texture, theme->scrollBar.arrowRect, theme)),
		mArrowDownRight(new ImageButton(static_cast<Widget::Ptr>(this), gui, mArrowDownRightName, theme->texture, theme->scrollBar.arrowRect, theme))
	{
		mThumb->setReactive(theme->scrollBar.reactiveThumb);
		mArrowUpLeft->setReactive(theme->scrollBar.reactiveArrows);
		mArrowDownRight->setReactive(theme->scrollBar.reactiveArrows);

		mThumb->setBorderRendered(false);
		mArrowUpLeft->setBorderRendered(false);
		mArrowDownRight->setBorderRendered(false);

		mThumb->setDraggable(true);

		addChild(mThumb, mThumbName);
		addChild(mArrowUpLeft, mArrowUpLeftName);
		addChild(mArrowDownRight, mArrowDownRightName);

		mRect.setTexture(theme->texture);
		mRect.setTextureRect(theme->scrollBar.backgroundRect);

		mThumb->bindCallback(GuiEvent::EventType::MouseDrag, mScrollBarCallbacks.OnThumbDrag, this, mScrollBarCallbacks.mCallbackIndices);
		mArrowUpLeft->bindCallback(GuiEvent::EventType::MouseButtonPressed, mScrollBarCallbacks.OnUpLeftArrowClick, this, mScrollBarCallbacks.mCallbackIndices);
		mArrowDownRight->bindCallback(GuiEvent::EventType::MouseButtonPressed, mScrollBarCallbacks.OnDownRightArrowClick, this, mScrollBarCallbacks.mCallbackIndices);

		setSize(50, 300);

		if (mVertical)
		{
			sf::Vector2f originUp(mArrowUpLeft->getSize().x / 2, mArrowUpLeft->getSize().y / 2);
			mArrowUpLeft->setOrigin(originUp);
			mArrowUpLeft->setRotation(0);

			sf::Vector2f originDown(mArrowDownRight->getSize().x / 2, mArrowDownRight->getSize().y / 2);
			mArrowDownRight->setOrigin(originDown);
			mArrowDownRight->setRotation(180);
		}
		
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

			if (mVertical)
			{
				sf::Vector2f originUp(mArrowUpLeft->getSize().x / 2, mArrowUpLeft->getSize().y / 2);
				mArrowUpLeft->setOrigin(originUp);
				mArrowUpLeft->setRotation(0);

				sf::Vector2f originDown(mArrowDownRight->getSize().x / 2, mArrowDownRight->getSize().y / 2);
				mArrowDownRight->setOrigin(originDown);
				mArrowDownRight->setRotation(180);
			}
			else
			{
				sf::Vector2f originUp(mArrowUpLeft->getSize().x / 2, mArrowUpLeft->getSize().y / 2);
				mArrowUpLeft->setOrigin(originUp);
				mArrowUpLeft->setRotation(-90);

				sf::Vector2f originDown(mArrowDownRight->getSize().x / 2, mArrowDownRight->getSize().y / 2);
				mArrowDownRight->setOrigin(originDown);
				mArrowDownRight->setRotation(90);
			}

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
		mArrowUpLeft->enable();
		mArrowDownRight->enable();

		mShowArrows = true;

		resizeThumb(getSize());

		setDirty();
		update();
	}

	void ScrollBar::hideArrows()
	{
		mArrowUpLeft->disable();
		mArrowDownRight->disable();

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

			if (mValue < 0 || mMaximumArea < mViewableArea)
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
				arrowSize = mArrowDownRight->getSize().y;
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
			arrowSize = mArrowDownRight->getSize().y;
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
			mArrowUpLeft->setSize(newSize.x, newSize.x);
			mArrowDownRight->setSize(newSize.x, newSize.x);

			mArrowUpLeft->setPosition(newSize.x * 0.5, newSize.x * 0.5);
			mArrowDownRight->setPosition(newSize.x * 0.5, newSize.x * 0.5 + newSize.y - newSize.x); // mRectsize.y - arrow size.x
		}
		else // Horizontal
		{
			mArrowUpLeft->setSize(newSize.y, newSize.y);
			mArrowDownRight->setSize(newSize.y, newSize.y);

			mArrowUpLeft->setPosition(newSize.y * 0.5, newSize.y * 0.5);
			mArrowDownRight->setPosition(newSize.y * 0.5 + newSize.x - newSize.y, newSize.y * 0.5); // mRectsize.x - arrow size.y
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
			arrowSize = mArrowDownRight->getSize().y;
		}

		if (mMoveFlag == false)
		{
			if (mVertical)
			{
				mThumb->setPosition(0, calculateThumbPos(arrowSize));

				sf::Vector2f upLeft = mArrowUpLeft->getSize();
				mArrowUpLeft->setPosition(upLeft.x * 0.5, upLeft.y * 0.5);
				sf::Vector2f downRight = mArrowDownRight->getSize();
				mArrowDownRight->setPosition(downRight.x * 0.5, downRight.y * 0.5 + mRect.getSize().y - arrowSize); // mRectsize.y - arrow size.x
			}
			else // Horizontal
			{
				mThumb->setPosition(calculateThumbPos(arrowSize), 0);

				sf::Vector2f upLeft = mArrowUpLeft->getSize();
				mArrowUpLeft->setPosition(upLeft.x * 0.5, upLeft.y * 0.5);
				sf::Vector2f downRight = mArrowDownRight->getSize();
				mArrowDownRight->setPosition(downRight.x * 0.5 + mRect.getSize().x - arrowSize, downRight.x * 0.5); // mRectsize.x - arrow size.y
			}
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

			target.draw(*mArrowUpLeft, states);
			target.draw(*mArrowDownRight, states);

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