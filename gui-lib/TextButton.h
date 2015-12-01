#ifndef GUITEXTBUTTON_H
#define GUITEXTBUTTON_H

#include "BorderWidget.h"
#include "Label.h"

namespace gui
{
	struct Theme;

	class TextButton : public BorderWidget
	{
	public:
		using Ptr = std::shared_ptr<TextButton>;

		TextButton(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& text, Theme* theme);
		~TextButton();

		// Changes the text of the button
		void setText(const std::string& str);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		// To update graphic pos
		virtual void update() override;
	private:
		// Text of the button
		Label::Ptr mText;
	};
} // namespace
#endif
