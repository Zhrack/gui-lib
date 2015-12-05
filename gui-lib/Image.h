#ifndef GUIIMAGE_H
#define GUIIMAGE_H

#include "Widget.h"

namespace gui
{
	struct Theme;

	class Image : public Widget
	{
	public:
		using Ptr = std::shared_ptr<Image>;

		Image(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Texture* tex = nullptr, sf::IntRect& rect = sf::IntRect());
		~Image();

		// Changes the texture of the Widget
		// If resetRect is true, the clip rect will be adjusted to the size of the texture
		// Else it is left unchanged
		void setTexture(sf::Texture* tex, bool resetRect = false);

		// Defines a new clip rect inside the texture
		void setTextureRect(const sf::IntRect& rect);

		void resize(sf::Vector2f newSize);
		void resize(float x, float y);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	private:

		friend class ImageButton;
	};
}
#endif