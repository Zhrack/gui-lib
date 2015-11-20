#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "Widget.h"
#include <SFML\Graphics\VertexArray.hpp>
#include <SFML\Graphics\Texture.hpp>

namespace gui
{
	struct Theme;

	// Manages resize of button through 9-patch
	class BorderWidget : public Widget
	{
	public:
		using Ptr = std::shared_ptr<BorderWidget>;

		BorderWidget(const Widget::Ptr& parent, Gui* const gui, const std::string& name, Theme* theme);
		~BorderWidget();

		void updateNinePatchPoints(sf::FloatRect& rect1, sf::FloatRect& rect2);
		void updateNinePatchPoints(sf::Vector2f* points);

		void updateVertsPosition();

	protected:
		// Vertices of the 9-patch
		sf::VertexArray mVerts;

		// Ptr to the theme texture
		sf::Texture* mTexture;

		// Margins to get position of some verts
		sf::FloatRect mInternalMargins;
	};
}
#endif