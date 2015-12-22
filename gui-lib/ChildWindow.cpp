#include "ChildWindow.h"
#include "Theme.h"

namespace gui
{
	const std::string ChildWindow::mBodyName = "body";
	const std::string ChildWindow::mTitleBarName = "titleBar";
	const std::string ChildWindow::mTitleName = "title";
	const std::string ChildWindow::mButtonName = "closeButton";

	ChildWindow::ChildWindow(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& title, Theme* theme) :
		BorderWidget(parent, gui, name, theme, false),
		mBody(new Panel(static_cast<Widget::Ptr>(this), gui, mBodyName)),
		mTitleBar(new Panel(static_cast<Widget::Ptr>(this), gui, mTitleBarName)),
		mTitle(new Label(static_cast<Widget::Ptr>(this), gui, mTitleName, title, theme)),
		mCloseButton(new ImageButton(static_cast<Widget::Ptr>(this), gui, mButtonName, theme->texture, theme->childWindow.closeButtonRect, theme, theme->childWindow.reactive))
	{
		mTitleBar->getShape().setTexture(theme->texture);
		mTitleBar->getShape().setTextureRect(theme->childWindow.titleBarRect);
		mBody->getShape().setTexture(theme->texture);
		mBody->getShape().setTextureRect(theme->childWindow.bodyRect);

		mTitle->setCharacterSize(theme->childWindow.label.textSize);

		

		addChild(mCloseButton, mCloseButton->getName());

		mCloseButton->setReactive(theme->childWindow.reactive);

		setBorderRendered(theme->childWindow.renderBorder);
		setSize(mRect.getSize());
		setDirty();
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
		resizeButton(size);

		if (mBorderRendered)
		{
			mTitleBar->setSize(size.x, mInternalMargins.top);
			mTitleBar->setPosition(mInternalMargins.left, mInternalMargins.top);
			mBody->setSize(size.x, size.y - mTitleBar->getShape().getSize().y);
			mBody->setPosition(mInternalMargins.left, mInternalMargins.top + mTitleBar->getShape().getSize().y);
		}
		else
		{
			mTitleBar->setSize(size.x, mInternalMargins.top);
			mTitleBar->setPosition(mInternalMargins.left, mInternalMargins.top);
			mBody->setSize(size.x, size.y - mTitleBar->getShape().getSize().y);
			mBody->setPosition(0, mTitleBar->getShape().getSize().y);
		}
		
		mCloseButton->setSize(5, 5);

		setDirty();
	}

	void ChildWindow::setPosition(const sf::Vector2f& localPos)
	{
		mRect.setPosition(localPos + mParent->getGlobalPosition());
		updateVertsPosition();

		sf::Vector2f marginOffset;
		if (mBorderRendered)
		{
			marginOffset = sf::Vector2f(mInternalMargins.left, mInternalMargins.top);
		}

		mTitleBar->setPosition(marginOffset);

		mTitle->setPosition(marginOffset);

		mBody->setPosition(sf::Vector2f(marginOffset.x, marginOffset.y + mTitleBar->getShape().getSize().y));

		mCloseButton->setPosition(mTitleBar->getShape().getSize().x - mCloseButton->getShape().getSize().x, 0);
	}

	void ChildWindow::setPosition(float x, float y)
	{
		setPosition(sf::Vector2f(x, y));
	}

	void ChildWindow::setGlobalPosition(const sf::Vector2f& globalPos)
	{
		mRect.setPosition(globalPos);
		updateVertsPosition();

		sf::Vector2f marginOffset;
		if (mBorderRendered)
		{
			marginOffset = sf::Vector2f(mInternalMargins.left, mInternalMargins.top);
		}
		
		mTitleBar->setPosition(marginOffset);

		mTitle->setPosition(marginOffset);

		mBody->setPosition(sf::Vector2f(marginOffset.x, marginOffset.y + mTitleBar->getShape().getSize().y));

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

			if (mBorderRendered)
			{
				// Reposition text inside button
				mTitle->setPosition(mInternalMargins.left, mInternalMargins.top);

				mTitleBar->setPosition(mInternalMargins.left, mInternalMargins.top);

				mBody->setPosition(mInternalMargins.left, mInternalMargins.top + mTitleBar->getShape().getSize().y);
			}
			else
			{
				// Reposition text inside button
				mTitle->setPosition(0, 0);

				mTitleBar->setPosition(0, 0);

				mBody->setPosition(sf::Vector2f(0, mTitleBar->getShape().getSize().y));
			}

			mCloseButton->setPosition(mTitleBar->getShape().getSize().x - mCloseButton->getShape().getSize().x, 0);

			setClean();
		}
	}

	void ChildWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isEnabled() && isVisible())
		{
			target.draw(mRect, states); // debug
			if (mBorderRendered)
			{
				states.texture = mTexture;
				target.draw(mVerts, states);
				states = sf::RenderStates::Default;
			}
			target.draw(*mBody, states);
			target.draw(*mTitleBar, states);
			target.draw(*mTitle, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}
}