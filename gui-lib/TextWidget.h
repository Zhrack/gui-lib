#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include "Widget.h"

#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Text.hpp>

namespace gui
{
	class TextWidget : public Widget
	{
	public:
		using Ptr = std::shared_ptr<TextWidget>;

		TextWidget(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Font* font = nullptr);
		~TextWidget();

		void setText(const std::string& str);
		sf::Text& getString() { return mText; }

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		// To update bbox of mRect
		virtual void update() override;
		// Update mRect and mText after a change
		void updateTextTransform();

	private:
		sf::Text mText;

		friend class Button;
	};
}

#endif