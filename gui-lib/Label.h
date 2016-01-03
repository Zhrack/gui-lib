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

		enum TextWrap
		{
			Free,
			Horizontal
		};

		Label(
			const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& text, Theme* theme, 
			sf::Font* font = nullptr, TextWrap wrap = TextWrap::Free
			);
		~Label();

		void setText(const std::string& str);
		sf::Text& getString() { return mText; }

		TextWrap getWrapStyle() const { return mWrap; }
		// Set Wrap style
		// param: maxWidth, only for TextWrap::Horizontal, the max width of the text, in pixels
		void setWrapStyle(TextWrap wrap, int maxWidth = -1, const std::string& text = "");

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

		// Splits a string given the delimiter
		std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems);
		std::vector<std::string> split(const std::string& s, char delim);
	private:
		sf::Text mText;

		TextWrap mWrap;

		friend class TextButton;
	};
}

#endif