#include "ChildWindow.h"
#include "Theme.h"

namespace gui
{
	ChildWindow::ChildWindow(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& title, Theme* theme) :
		BorderWidget(parent, gui, name, theme, false),
		mBody(new Panel(static_cast<Widget::Ptr>(this), gui, "body")),
		mTitleBar(new Panel(static_cast<Widget::Ptr>(this), gui, "")),
		mTitle(new Label(static_cast<Widget::Ptr>(this), gui, "", title)),
		mCloseButton(new ImageButton(static_cast<Widget::Ptr>(this), gui, "close", theme->texture, theme->childWindow.closeButtonRect, theme, false))
	{
		addChild(mBody, mBody->getName());
		addChild(mCloseButton, mCloseButton->getName());

		mBody->getShape().setFillColor(sf::Color::Blue);
		mTitleBar->getShape().setFillColor(sf::Color::Yellow);
		mCloseButton->setDraggable(true);
	}


	ChildWindow::~ChildWindow()
	{
	}

	void ChildWindow::setTitleSize(unsigned int size)
	{
		mTitle->getString().setCharacterSize(size);

		setDirty();
	}

	void ChildWindow::setSize(float x, float y)
	{
		setSize(sf::Vector2f(x, y));
	}

	void ChildWindow::setSize(const sf::Vector2f& size)
	{
		mRect.setSize(size);

		mBody->getShape().setSize(sf::Vector2f(size.x, size.y - 20));
		mBody->setPosition(sf::Vector2f(0, 20));
		mTitleBar->getShape().setSize(sf::Vector2f(size.x, 20));
		mCloseButton->resize(5, 5);
		mCloseButton->setPosition(size.x - mCloseButton->getShape().getSize().x, 0);
		setDirty();
	}

	void ChildWindow::update()
	{
		if (isDirty())
		{
			updateVertsPosition();

			// Reposition text inside button
			mTitle->setGlobalPosition(sf::Vector2f(
				getGlobalPosition().x + mInternalMargins.left,
				getGlobalPosition().y + mInternalMargins.top)
				);

			mTitle->updateTextTransform();

			mTitleBar->setPosition(0, 0);
			mCloseButton->setPosition(mRect.getSize().x - mCloseButton->getShape().getSize().x, 0);
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
			target.draw(*mTitleBar, states);
			//target.draw(*mTitle, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}
}