#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "TextWidget.h"
#include <SFML\Graphics\VertexArray.hpp>
#include <SFML\Graphics\Texture.hpp>

namespace gui
{
	class Button : public Widget
	{
	public:
		using Ptr = std::shared_ptr<Button>;

		Button(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Vector2f* points, const std::string& text = "");
		~Button();

		void updateNinePatchPoints(sf::Vector2f* points);

		void updateVertsPosition();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		// Vertices of the 9-patch
		sf::VertexArray mVerts;

		// Texture from which the image is taken
		sf::Texture* mTexture;

		// Text of the button, if present
		TextWidget::Ptr mText;
	};
}
#endif