#include "TextButton.h"

#include "Gui.h"

namespace gui
{
	const std::string TextButton::mLabelName = "label";

	TextButton::TextButton(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& text, Theme* theme) :
		BorderWidget(parent, gui, name, theme, theme->button.reactive),
		mText(new Label(static_cast<Widget::Ptr>(this), gui, mLabelName, "", theme))
	{
		mText->getString().setColor(theme->button.label.textColor);
		mText->setCharacterSize(theme->button.label.textSize);
		setText(text);

		setBorderRendered(theme->button.renderBorder);
	}


	TextButton::~TextButton()
	{
	}

	void TextButton::setText(const std::string& str)
	{
		mText->setText(str);
		setDirty();
	}

	void TextButton::setCharacterSize(unsigned int size)
	{
		mText->setCharacterSize(size);

		setDirty();
	}

	void TextButton::setGlobalPosition(const sf::Vector2f& globalPos)
	{
		mRect.setPosition(globalPos);
		updateVertsPosition();

		sf::Vector2f marginOffset;
		if (mBorderRendered)
		{
			marginOffset = sf::Vector2f(mInternalMargins.left, mInternalMargins.top);
		}

		mText->setPosition(marginOffset);
	}

	void TextButton::setGlobalPosition(float x, float y)
	{
		setGlobalPosition(sf::Vector2f(x, y));
	}

	void TextButton::setTextWrapStyle(Label::TextWrap wrap, int maxWidth, const std::string& text)
	{
		mText->setWrapStyle(wrap, maxWidth, text);
		setDirty();
		update();
	}

	void TextButton::update()
	{
		if (isDirty())
		{
			// Resize button
			if (mBorderRendered)
			{
				mRect.setSize(sf::Vector2f(
					mInternalMargins.left + mText->getSize().x + mInternalMargins.width,
					mInternalMargins.top + mText->getSize().y + mInternalMargins.height
					));

				// Reposition text inside button
				mText->setPosition(mInternalMargins.left, mInternalMargins.top);
			}
			else
			{
				mRect.setSize(mText->mRect.getSize());

				// Reposition text inside button
				mText->setPosition(0, 0);
			}

			updateVertsPosition();

			setClean();
		}
	}

	void TextButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
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
			target.draw(*mText, states);

			mMainGui->setScissor(mText->mText.getGlobalBounds());

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}

			mMainGui->resetScissor();
		}
	}
} // namespace