#include "ChildWindow.h"
#include "Theme.h"

namespace gui
{
	ChildWindow::ChildWindow(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& title, Theme* theme) :
		BorderWidget(parent, gui, name, theme, false),
		mBody(new Panel(static_cast<Widget::Ptr>(this), gui, "body")),
		mTitleBar(new Panel(static_cast<Widget::Ptr>(this), gui, "titleBar")),
		mTitle(new Label(static_cast<Widget::Ptr>(this), gui, "title", title)),
		mCloseButton(new ImageButton(static_cast<Widget::Ptr>(this), gui, "closeButton", theme->texture, theme->childWindow.closeButtonRect, theme, false))
	{
		mBody->getShape().setFillColor(sf::Color::Blue);
		mTitleBar->getShape().setFillColor(sf::Color::Yellow);

		setSize(mRect.getSize());
		mCloseButton->setDraggable(true);

		addChild(mCloseButton, mCloseButton->getName());
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

		mTitleBar->getShape().setSize(sf::Vector2f(size.x - mInternalMargins.left - mInternalMargins.width, mInternalMargins.top));
		mTitleBar->setPosition(mInternalMargins.left, mInternalMargins.top);

		mBody->getShape().setSize(sf::Vector2f(
			size.x - mInternalMargins.left - mInternalMargins.width,
			size.y - mInternalMargins.top - mInternalMargins.width - mTitleBar->getShape().getSize().y)
			);
		mBody->setPosition(sf::Vector2f(mInternalMargins.left, mInternalMargins.top + mTitleBar->getShape().getSize().y));
		
		mCloseButton->resize(5, 5);
		mCloseButton->setPosition(mRect.getSize().x - mCloseButton->getShape().getSize().x, 0);
	}

	void ChildWindow::setPosition(const sf::Vector2f& localPos)
	{
		mRect.setPosition(localPos + mParent->getGlobalPosition());
		updateVertsPosition();

		mTitleBar->setPosition(mInternalMargins.left, mInternalMargins.top);

		mBody->setPosition(sf::Vector2f(mInternalMargins.left, mInternalMargins.top + mTitleBar->getShape().getSize().y));

		mCloseButton->setPosition(mRect.getSize().x - mCloseButton->getShape().getSize().x, 0);
	}

	void ChildWindow::setPosition(float x, float y)
	{
		setPosition(sf::Vector2f(x, y));
	}

	void ChildWindow::setGlobalPosition(const sf::Vector2f& globalPos)
	{
		mRect.setPosition(globalPos);
		updateVertsPosition();
		
		mTitleBar->setPosition(mInternalMargins.left, mInternalMargins.top);

		mBody->setPosition(sf::Vector2f(mInternalMargins.left, mInternalMargins.top + mTitleBar->getShape().getSize().y));

		mCloseButton->setPosition(mRect.getSize().x - mCloseButton->getShape().getSize().x, 0);
	}

	void ChildWindow::setGlobalPosition(float x, float y)
	{
		setGlobalPosition(sf::Vector2f(x, y));
	}

	void ChildWindow::update()
	{
		if (isDirty())
		{
			updateVertsPosition();

			// Reposition text inside button
			mTitle->setPosition(mInternalMargins.left, mInternalMargins.top);
			mTitle->updateTextTransform();

			mTitleBar->setPosition(mInternalMargins.left, mInternalMargins.top);

			mBody->setPosition(mInternalMargins.left, mInternalMargins.top + mTitleBar->getShape().getSize().y);

			mCloseButton->setPosition(mTitleBar->getShape().getSize().x - mCloseButton->getShape().getSize().x, 0);

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
			target.draw(*mBody, states);
			target.draw(*mTitleBar, states);
			//target.draw(*mTitle, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}
}