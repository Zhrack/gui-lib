#include "TextButton.h"

#include "Gui.h"

namespace gui
{
	const std::string TextButton::mLabelName = "label";

	TextButton::TextButton(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& text, Theme* theme, bool reactive) :
		BorderWidget(parent, gui, name, theme, theme->button.reactive),
		mText(new Label(static_cast<Widget::Ptr>(this), gui, mLabelName, ""))
	{
		setText(text);
		mText->getString().setColor(theme->button.label.textColor);
		//updateNinePatchPoints(theme->textButton.texRect, theme->textButton.internalMargins);
		//toNormalButtonState();
	}


	TextButton::~TextButton()
	{
	}

	void TextButton::setText(const std::string& str)
	{
		mText->setText(str);

		// Resize button
		mRect.setSize(sf::Vector2f(
			mInternalMargins.left + mText->mRect.getSize().x + mInternalMargins.width,
			mInternalMargins.top + mText->mRect.getSize().y + mInternalMargins.height
			));

		updateVertsPosition();

		// Reposition text inside button
		mText->setGlobalPosition(sf::Vector2f(
			getGlobalPosition().x + mInternalMargins.left,
			getGlobalPosition().y + mInternalMargins.top)
			);

	}

	void TextButton::setCharacterSize(unsigned int size)
	{
		mText->getString().setCharacterSize(size);

		setDirty();
	}

	void TextButton::setPosition(const sf::Vector2f& localPos)
	{
		mRect.setPosition(localPos + mParent->getGlobalPosition());
		updateVertsPosition();

		mText->setPosition(mInternalMargins.left, mInternalMargins.top);
	}

	void TextButton::setPosition(float x, float y)
	{
		setPosition(sf::Vector2f(x, y));
	}

	void TextButton::setGlobalPosition(const sf::Vector2f& globalPos)
	{
		mRect.setPosition(globalPos);
		updateVertsPosition();

		mText->setPosition(mInternalMargins.left, mInternalMargins.top);
	}

	void TextButton::setGlobalPosition(float x, float y)
	{
		setGlobalPosition(sf::Vector2f(x, y));
	}

	void TextButton::update()
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

	void TextButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isEnabled() && isVisible())
		{
			//target.draw(mRect, states); // debug
			states.texture = mTexture;
			target.draw(mVerts, states);
			states = sf::RenderStates::Default;
			target.draw(*mText, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}
} // namespace