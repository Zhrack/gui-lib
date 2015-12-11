#ifndef GUICHILDWINDOW_H
#define GUICHILDWINDOW_H

#include "BorderWidget.h"
#include "Panel.h"
#include "Label.h"
#include "ImageButton.h"

namespace gui
{
	class ChildWindow : public BorderWidget
	{
	public:
		using Ptr = std::shared_ptr<ChildWindow>;

		ChildWindow(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& title, Theme* theme);
		~ChildWindow();

		Panel::Ptr getBody() const { return mBody; }
		Label::Ptr getTitle() const { return mTitle; }
		ImageButton::Ptr getCloseButton() const { return mCloseButton; }

		// Changes title
		void setTitle(const std::string& title) { mTitle->setText(title); }
		// Default value is 30
		void setTitleSize(unsigned int size);
		// Set a new size for the window
		void setSize(const sf::Vector2f& size);
		void setSize(float x, float y);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	private:
		// To update graphic pos
		virtual void update() override;
	private:
		// Body of the window
		Panel::Ptr mBody;
		Panel::Ptr mTitleBar;
		Label::Ptr mTitle;
		ImageButton::Ptr mCloseButton;
	};

}// namespace
#endif