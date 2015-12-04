#include "TextButton.h"

#include "Gui.h"

namespace gui
{
	TextButton::TextButton(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& text, Theme* theme) :
		BorderWidget(parent, gui, name, theme),
		mText(new Label(static_cast<Widget::Ptr>(this), gui, "", ""))
	{
		setText(text);
		mText->getString().setColor(theme->textButton.label.textColor);
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

	void TextButton::update()
	{
		updateVertsPosition();

		// Reposition text inside button
		mText->setGlobalPosition(sf::Vector2f(
			getGlobalPosition().x + mInternalMargins.left,
			getGlobalPosition().y + mInternalMargins.top)
			);

		mText->updateTextTransform();
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