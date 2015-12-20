#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"

#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Text.hpp>

namespace gui
{
	struct Theme;

	class Label : public Widget
	{
	public:
		using Ptr = std::shared_ptr<Label>;

		Label(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& text, Theme* theme, sf::Font* font = nullptr);
		~Label();

		void setText(const std::string& str);
		sf::Text& getString() { return mText; }

		// Default value is 30
		void setCharacterSize(unsigned int size);

		

		void setPosition(const sf::Vector2f& localPos) override;
		void setPosition(float x, float y) override;
		void setGlobalPosition(const sf::Vector2f& globalPos) override;
		void setGlobalPosition(float x, float y) override;

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