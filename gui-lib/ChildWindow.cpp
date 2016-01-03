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
		mCloseButton(new ImageButton(static_cast<Widget::Ptr>(this), gui, mButtonName, theme->texture, theme->childWindow.closeButtonRect, theme))
	{
		mTitleBar->getShape().setTexture(theme->texture);
		mTitleBar->getShape().setTextureRect(theme->childWindow.titleBarRect);
		mBody->getShape().setTexture(theme->texture);
		mBody->getShape().setTextureRect(theme->childWindow.bodyRect);

		setTitleSize(theme->childWindow.label.textSize);
		mTextLayout.mCurrentLayout = TextLayout::Center;

		addChild(mCloseButton, mCloseButton->getName());

		mCloseButton->setReactive(theme->childWindow.reactive);
		mCloseButton->setBorderRendered(theme->childWindow.closeButtonBorderRendered);

		setBorderRendered(theme->childWindow.renderBorder);
		setSize(mRect.getSize());
		setDirty();
	}


	ChildWindow::~ChildWindow()
	{
	}

	void ChildWindow::setTitleSize(unsigned int size)
	{
		mTitle->setCharacterSize(size);

		mTitleBar->setSize(getSize().x, mTitle->getSize().y);

		setDirty();
	}

	void ChildWindow::setButtonPos(const sf::Vector2f& localPos)
	{
		mCloseButton->setPosition(localPos);
	}

	void ChildWindow::setTextLayout(const TextLayout& layout)
	{
		mTextLayout.mCurrentLayout = layout;
	}

	void ChildWindow::setSize(float x, float y)
	{
		setSize(sf::Vector2f(x, y));
	}

	void ChildWindow::recalculateTextLayout()
	{
		mTextLayout.mCenterLayoutMargin = sf::Vector2f((mTitleBar->getSize().x - mTitle->getSize().x) / 2, 0);
		mTextLayout.mLeftLayoutMargin = sf::Vector2f(5, 0);
		mTextLayout.mRightLayoutMargin = sf::Vector2f(mTitleBar->getSize().x - mTitle->getSize().x, 0);
	}

	void ChildWindow::setSize(const sf::Vector2f& size)
	{
		resizeButton(size);

		if (mBorderRendered)
		{
			mTitleBar->setSize(size.x, mTitleBar->getSize().y);
			mTitleBar->setPosition(mInternalMargins.left, mInternalMargins.top);
			mBody->setSize(size.x, size.y - mTitleBar->getShape().getSize().y);
			mBody->setPosition(mInternalMargins.left, mInternalMargins.top + mTitleBar->getShape().getSize().y);
		}
		else
		{
			mTitleBar->setSize(size.x, mTitleBar->getSize().y);
			mTitleBar->setPosition(mInternalMargins.left, mInternalMargins.top);
			mBody->setSize(size.x, size.y - mTitleBar->getShape().getSize().y);
			mBody->setPosition(0, mTitleBar->getShape().getSize().y);
		}
		
		mCloseButton->setSize(mTitleBar->getSize().y, mTitleBar->getSize().y);

		// Recalculate text margins
		recalculateTextLayout();

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

		sf::Vector2f textMargin;
		switch (mTextLayout.mCurrentLayout)
		{
		case Left:
			textMargin = mTextLayout.mLeftLayoutMargin;
			break;
		case Center:
			textMargin = mTextLayout.mCenterLayoutMargin;
			break;
		//case Right:
		//	textMargin = mTextLayout.mRightLayoutMargin;
		//	break;
		}
		mTitle->setPosition(marginOffset + textMargin);

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

		sf::Vector2f textMargin;
		switch (mTextLayout.mCurrentLayout)
		{
		case Left:
			textMargin = mTextLayout.mLeftLayoutMargin;
			break;
		case Center:
			textMargin = mTextLayout.mCenterLayoutMargin;
			break;
		//case Right:
		//	textMargin = mTextLayout.mRightLayoutMargin;
		//	break;
		}
		mTitle->setPosition(marginOffset + textMargin);

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
				sf::Vector2f textMargin;
				switch (mTextLayout.mCurrentLayout)
				{
				case Left:
					textMargin = mTextLayout.mLeftLayoutMargin;
					break;
				case Center:
					textMargin = mTextLayout.mCenterLayoutMargin;
					break;
				//case Right:
				//	textMargin = mTextLayout.mRightLayoutMargin;
				//	break;
				}
				textMargin += sf::Vector2f(mInternalMargins.left, mInternalMargins.top);
				mTitle->setPosition(textMargin);

				mTitleBar->setPosition(mInternalMargins.left, mInternalMargins.top);

				mBody->setPosition(mInternalMargins.left, mInternalMargins.top + mTitleBar->getShape().getSize().y);
			}
			else
			{
				// Reposition text inside button
				sf::Vector2f textMargin;
				switch (mTextLayout.mCurrentLayout)
				{
				case Left:
					textMargin = mTextLayout.mLeftLayoutMargin;
					break;
				case Center:
					textMargin = mTextLayout.mCenterLayoutMargin;
					break;
				//case Right:
				//	textMargin = mTextLayout.mRightLayoutMargin;
				//	break;
				}
				mTitle->setPosition(textMargin);

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
			//target.draw(mRect, states); // debug
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