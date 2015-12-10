#include "ChildWindow.h"
#include "Theme.h"

namespace gui
{
	ChildWindow::ChildWindow(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& title, Theme* theme) :
		BorderWidget(parent, gui, name, theme, false),
		mBody(new Panel(static_cast<Widget::Ptr>(this), gui, "body")),
		mTitleBar(new Panel(static_cast<Widget::Ptr>(this), gui, "")),
		mTitle(new Label(static_cast<Widget::Ptr>(this), gui, "", title)),
		mCloseButton(new ImageButton(parent, gui, "close", theme->texture, imageRect, theme))
	{
		addChild(mBody, mBody->getName());
		addChild(mCloseButton, mCloseButton->getName());
	}


	ChildWindow::~ChildWindow()
	{
	}

	void ChildWindow::update()
	{
		if (isDirty())
		{
			updateVertsPosition();

			// Reposition text inside button
			mText->setGlobalPosition(sf::Vector2f(
				getGlobalPosition().x + mInternalMargins.left,
				getGlobalPosition().y + mInternalMargins.top)
				);

			mText->updateTextTransform();
			setClean();
		}
	}

	void ChildWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isEnabled() && isVisible())
		{
			//target.draw(mRect, states); // debug
			states.texture = mTexture;
			target.draw(mVerts, states);
			states = sf::RenderStates::Default;
			target.draw(*mTitle, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}
}