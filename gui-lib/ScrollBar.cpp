#include "ScrollBar.h"

namespace gui
{
	const std::string ScrollBar::mThumbName = "thumb";
	const std::string ScrollBar::mArrowUpRightName = "arrowUpRight";
	const std::string ScrollBar::mArrowDownLeftName = "arrowDownLeft";

	ScrollBar::ScrollBar(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Texture* texture, sf::IntRect& imageRect, Theme* theme) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(100, 15), true, true, false, true, false),
		
		mValue(0),
		mViewableArea(5),
		mMaximumArea(10),
		mScrollAmount(1),

		mAutoHide(true),
		mVertical(true),
		mShowArrows(true),

		mScrollableWidget(nullptr),
		mThumb(new ImageButton(this, gui, mThumbName, theme, theme, theme)),
		mArrowUpRight(new ImageButton(this, gui, mArrowUpRightName, theme, theme, theme)),
		mArrowDownLeft(new ImageButton(this, gui, mArrowDownLeftName, theme, theme, theme))
	{

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
			setPosition(mParent->getSize().x - size.y, 0);
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

			// Reposition thumb
			if (mVertical)
			{
				mThumb->setPosition(0, )
			}
			else // Horizontal
			{

			}
		}
	}

	void ScrollBar::setSize(const sf::Vector2f& newSize)
	{

	}

	void ScrollBar::setSize(float x, float y)
	{

	}

	void ScrollBar::setPosition(const sf::Vector2f& localPos)
	{

	}

	void ScrollBar::setPosition(float x, float y)
	{

	}

	void ScrollBar::setGlobalPosition(const sf::Vector2f& globalPos)
	{

	}

	void ScrollBar::setGlobalPosition(float x, float y)
	{

	}


	void ScrollBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{

	}

	void ScrollBar::update()
	{

	}

} // namespace