#ifndef GUISCROLLBAR_H
#define GUISCROLLBAR_H

#include "Widget.h"
#include "ImageButton.h"

namespace gui
{
	using uint = unsigned int;

	class ScrollBar : public Widget
	{
	public:
		using Ptr = std::shared_ptr<ScrollBar>;

		// Child names
		static const std::string mThumbName;
		static const std::string mArrowUpRightName;
		static const std::string mArrowDownLeftName;

		ScrollBar(const Widget::Ptr& parent, Gui* const gui, const std::string& name, Theme* theme);
		~ScrollBar();

		// Binds a widget to the scrollbar, its children will be scrolled
		void bindScrollableWidget(const Widget::Ptr& widget, uint viewableArea, uint maximumArea);

		// Sets the scrollbar layout
		void setVertical(bool vertical);

		void setViewableArea(uint viewableArea);

		void setMaximumArea(uint maximum);

		// Sets the amount of scroll of the arrows, when pressed
		void setScrollAmount(uint amount);

		// Decides whether to show arrows
		void showArrows();
		void hideArrows();

		// Sets the current value, clamped to [mViewableArea, mMaximumArea] or 0
		void setValue(uint value);

		bool isVertical() const { return mVertical; }
		uint getScrollAmount() const { return mScrollAmount; }
		bool isArrowShown() const { return mShowArrows; }
		uint getValue() const { return mValue; }
		uint getViewableArea() const { return mViewableArea; }
		uint getMaximumArea() const { return mMaximumArea; }

		void setSize(const sf::Vector2f& newSize) override;
		void setSize(float x, float y) override;

		void setPosition(const sf::Vector2f& localPos) override;
		void setPosition(float x, float y) override;
		void setGlobalPosition(const sf::Vector2f& globalPos) override;
		void setGlobalPosition(float x, float y) override;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		virtual void update() override;

		void resizeThumb(const sf::Vector2f& newSize);

		// Small utility function to avoid code duplication
		int calculateThumbPos(int arrowSize);

	private:
		// Bound widget
		Widget::Ptr mScrollableWidget;

		// Draggable thumb
		ImageButton::Ptr mThumb;

		// Can be the Up arrow for vertical mode, Right arrow otherwise
		ImageButton::Ptr mArrowUpRight;
		// Can be the Down arrow for vertical mode, Left arrow otherwise
		ImageButton::Ptr mArrowDownLeft;

		// Does the scrollbar lies vertically?
		bool mVertical;

		// Does the scrollbar hides automatically when not needed?
		bool mAutoHide;

		// Are arrows needed?
		bool mShowArrows;

		// Pixels of the viewable area
		// E.g.: in case of a list box: the height of the box
		uint mViewableArea;

		// Maximum pixels of scroll
		// E.g.: in case of a list box: the sum of the heights of the list items inside
		uint mMaximumArea;

		// Current value of the scrollbar, must be inside [mViewableArea, mMaximumArea] or equals 0
		uint mValue;

		// How much does arrows scroll when pressed?
		uint mScrollAmount;
	};
} // namespace

#endif