#ifndef GUIIMAGEBUTTON_H
#define GUIIMAGEBUTTON_H

#include "Button.h"
#include "Image.h"

namespace gui
{
	class ImageButton : public Button
	{
	public:
		using Ptr = std::shared_ptr<ImageButton>;

		ImageButton(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Vector2f* points, sf::Texture* tex);
		~ImageButton();

		// Changes the texture of the button
		// If points == nullptr, internal margins of the texture will be reused
		void setTexture(sf::Texture* tex, sf::Vector2f* points = nullptr);

		Image::Ptr getImage() const { return mImage; }

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	private:
		// Image of the button
		Image::Ptr mImage;
	};
} // namespace
#endif