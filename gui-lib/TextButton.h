#ifndef GUITEXTBUTTON_H
#define GUITEXTBUTTON_H

#include "Button.h"
#include "Label.h"

namespace gui
{
	class TextButton : public Button
	{
	public:
		using Ptr = std::shared_ptr<TextButton>;

		TextButton(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Vector2f* points, const std::string& text);
		~TextButton();

		// Changes the text of the button
		void setText(const std::string& str);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		// Text of the button
		Label::Ptr mText;
	};
} // namespace
#endif
