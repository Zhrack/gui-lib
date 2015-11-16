#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"

#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Text.hpp>

namespace gui
{
	class Label : public Widget
	{
	public:
		using Ptr = std::shared_ptr<Label>;

		Label(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Font* font = nullptr, const std::string& theme = "");
		~Label();

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

		friend class TextButton;
	};
}

#endif