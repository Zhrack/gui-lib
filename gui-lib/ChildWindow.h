#ifndef GUICHILDWINDOW_H
#define GUICHILDWINDOW_H

#include "BorderWidget.h"
#include "Panel.h"
#include "Label.h"
#include "ImageButton.h"

namespace gui
{
	enum TextLayout
	{
		Left,
		Center
		//Right
	};

	struct WindowTextLayout
	{
		TextLayout mCurrentLayout;
		sf::Vector2f mLeftLayoutMargin;
		sf::Vector2f mCenterLayoutMargin;
		sf::Vector2f mRightLayoutMargin;
	};

	class ChildWindow : public BorderWidget
	{
	public:
		using Ptr = std::shared_ptr<ChildWindow>;

		// Child names
		static const std::string mBodyName;
		static const std::string mTitleBarName;
		static const std::string mTitleName;
		static const std::string mButtonName;

		ChildWindow(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& title, Theme* theme);
		~ChildWindow();

		Panel::Ptr getBody() const { return mBody; }
		Label::Ptr getTitle() const { return mTitle; }

		// Changes title
		void setTitle(const std::string& title) { mTitle->setText(title); }
		// Default value is 30
		void setTitleSize(unsigned int size);

		void setButtonPos(const sf::Vector2f& localPos);

		// set general position of text
		void setTextLayout(const TextLayout& layout);

		// Set a new size for the window
		void setSize(const sf::Vector2f& size) override;
		void setSize(float x, float y) override;

		void setGlobalPosition(const sf::Vector2f& globalPos) override;
		void setGlobalPosition(float x, float y) override;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	private:
		void recalculateTextLayout();
		// To update graphic pos
		virtual void update() override;
	private:
		// Body of the window
		Panel::Ptr mBody;
		Panel::Ptr mTitleBar;

		Label::Ptr mTitle;
		WindowTextLayout mTextLayout;

		ImageButton::Ptr mCloseButton;
	};

}// namespace
#endif