#ifndef GUIIMAGEBUTTON_H
#define GUIIMAGEBUTTON_H

#include "BorderWidget.h"
#include "Image.h"

namespace gui
{
	struct Theme;

	class ImageButton : public BorderWidget
	{
	public:
		using Ptr = std::shared_ptr<ImageButton>;

		static const std::string mImageName;

		ImageButton(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Texture* texture, sf::IntRect& imageRect, Theme* theme, bool reactive = true);
		~ImageButton();

		// Changes the texture of the button
		// If points == nullptr, internal margins of the texture will be reused
		void setTexture(sf::Texture* tex, const sf::IntRect& clipRect);
		// Defines a new clip rect inside the texture
		void setTextureRect(const sf::IntRect& rect);

		// Resizes the image.
		// newSize = new pixel size of the image
		void setSize(const sf::Vector2f& newSize) override;
		void setSize(float x, float y) override;

		void setPosition(const sf::Vector2f& localPos) override;
		void setPosition(float x, float y) override;
		void setGlobalPosition(const sf::Vector2f& globalPos) override;
		void setGlobalPosition(float x, float y) override;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		virtual void update() override;
	private:
		// Image of the button
		Image::Ptr mImage;
	};
} // namespace
#endif