#include "BorderWidget.h"
#include "Theme.h"

#include <assert.h>

namespace gui
{
	// Ctor for Text Button
	BorderWidget::BorderWidget(const Widget::Ptr& parent, Gui* const gui, const std::string& name, Theme* theme) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(50, 50), true, true, false, true, false),
		mVerts(sf::PrimitiveType::Quads, 9 * 4),
		mTexture(theme->texture)
	{
		sf::Vector2f points[4];
		points[0] = sf::Vector2f(theme->textButton.texRect.left, theme->textButton.texRect.top);
		points[1] = sf::Vector2f(theme->textButton.internalMargins.left, theme->textButton.internalMargins.top);
		points[2] = sf::Vector2f(theme->textButton.internalMargins.width, theme->textButton.internalMargins.height);
		points[3] = sf::Vector2f(theme->textButton.texRect.width, theme->textButton.texRect.height);

		updateNinePatchPoints(points);
	}


	BorderWidget::~BorderWidget()
	{
		// Data is destroyed by ThemeCache
		mTexture = nullptr;
	}

	//	Points must be an array of size 4, with coords of 4 points in pixel space of the texture
	//	indicated by *, the rest is calculated from them.
	//	*--+--+--+
	//	|  |  |  |
	//	+--*--+--+
	//	|  |  |  |
	//	+--+--*--+
	//	|  |  |  |
	//	+--+--+--*
	void BorderWidget::updateNinePatchPoints(sf::Vector2f* points)
	{
		assert(points != nullptr);

		sf::Vector2f first = points[0];
		sf::Vector2f second = points[1];
		sf::Vector2f third = points[2];
		sf::Vector2f fourth = points[3];

		// Calculate texcoords

		// 0 top left
		mVerts[0 + 0].texCoords = sf::Vector2f(first);
		mVerts[0 + 1].texCoords = sf::Vector2f(second.x, first.y);
		mVerts[0 + 2].texCoords = sf::Vector2f(second);
		mVerts[0 + 3].texCoords = sf::Vector2f(first.x, second.y);

		// 1 top center
		mVerts[4 + 0].texCoords = sf::Vector2f(second.x, first.y);
		mVerts[4 + 1].texCoords = sf::Vector2f(third.x, first.y);
		mVerts[4 + 2].texCoords = sf::Vector2f(third.x, second.y);
		mVerts[4 + 3].texCoords = sf::Vector2f(second);

		// 2 top right
		mVerts[8 + 0].texCoords = sf::Vector2f(third.x, first.y);
		mVerts[8 + 1].texCoords = sf::Vector2f(fourth.x, first.y);
		mVerts[8 + 2].texCoords = sf::Vector2f(fourth.x, second.y);
		mVerts[8 + 3].texCoords = sf::Vector2f(third.x, second.y);

		// 3 left
		mVerts[12 + 0].texCoords = sf::Vector2f(first.x, second.y);
		mVerts[12 + 1].texCoords = sf::Vector2f(second);
		mVerts[12 + 2].texCoords = sf::Vector2f(second.x, third.y);
		mVerts[12 + 3].texCoords = sf::Vector2f(first.x, fourth.y);

		// 4 center
		mVerts[16 + 0].texCoords = sf::Vector2f(second);
		mVerts[16 + 1].texCoords = sf::Vector2f(third.x, second.y);
		mVerts[16 + 2].texCoords = sf::Vector2f(third);
		mVerts[16 + 3].texCoords = sf::Vector2f(second.x, third.y);

		// 5 right
		mVerts[20 + 0].texCoords = sf::Vector2f(third.x, second.y);
		mVerts[20 + 1].texCoords = sf::Vector2f(fourth.x, second.y);
		mVerts[20 + 2].texCoords = sf::Vector2f(fourth.x, third.y);
		mVerts[20 + 3].texCoords = sf::Vector2f(third);

		// 6 bottom left
		mVerts[24 + 0].texCoords = sf::Vector2f(first.x, third.y);
		mVerts[24 + 1].texCoords = sf::Vector2f(second.x, third.y);
		mVerts[24 + 2].texCoords = sf::Vector2f(second.x, fourth.y);
		mVerts[24 + 3].texCoords = sf::Vector2f(first.x, fourth.y);

		// 7 bottom center
		mVerts[28 + 0].texCoords = sf::Vector2f(second.x, third.y);
		mVerts[28 + 1].texCoords = sf::Vector2f(third);
		mVerts[28 + 2].texCoords = sf::Vector2f(third.x, fourth.y);
		mVerts[28 + 3].texCoords = sf::Vector2f(second.x, fourth.y);

		//  8 bottom right
		mVerts[32 + 0].texCoords = sf::Vector2f(third);
		mVerts[32 + 1].texCoords = sf::Vector2f(fourth.x, third.y);
		mVerts[32 + 2].texCoords = sf::Vector2f(fourth);
		mVerts[32 + 3].texCoords = sf::Vector2f(third.x, fourth.y);

		mInternalMargins.left = second.x - first.x;
		mInternalMargins.top = second.y - first.y;
		mInternalMargins.width = fourth.x - third.x;
		mInternalMargins.height = fourth.y - third.y;

		// Update verts position
		updateVertsPosition();
	}

	void BorderWidget::updateVertsPosition()
	{
		sf::FloatRect rect = mRect.getGlobalBounds();
		sf::Vector2f first = sf::Vector2f(rect.left, rect.top);
		sf::Vector2f second = sf::Vector2f(rect.left + mInternalMargins.left, rect.top + mInternalMargins.top);
		sf::Vector2f third = sf::Vector2f(rect.left + rect.width - mInternalMargins.width, rect.top + rect.height - mInternalMargins.height);
		sf::Vector2f fourth = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);

		// 0 top left
		mVerts[0 + 0].position = sf::Vector2f(first);
		mVerts[0 + 1].position = sf::Vector2f(second.x, first.y);
		mVerts[0 + 2].position = sf::Vector2f(second);
		mVerts[0 + 3].position = sf::Vector2f(first.x, second.y);

		// 1 top center
		mVerts[4 + 0].position = sf::Vector2f(second.x, first.y);
		mVerts[4 + 1].position = sf::Vector2f(third.x, first.y);
		mVerts[4 + 2].position = sf::Vector2f(third.x, second.y);
		mVerts[4 + 3].position = sf::Vector2f(second);

		// 2 top right
		mVerts[8 + 0].position = sf::Vector2f(third.x, first.y);
		mVerts[8 + 1].position = sf::Vector2f(fourth.x, first.y);
		mVerts[8 + 2].position = sf::Vector2f(fourth.x, second.y);
		mVerts[8 + 3].position = sf::Vector2f(third.x, second.y);

		// 3 left
		mVerts[12 + 0].position = sf::Vector2f(first.x, second.y);
		mVerts[12 + 1].position = sf::Vector2f(second);
		mVerts[12 + 2].position = sf::Vector2f(second.x, third.y);
		mVerts[12 + 3].position = sf::Vector2f(first.x, fourth.y);

		// 4 center
		mVerts[16 + 0].position = sf::Vector2f(second);
		mVerts[16 + 1].position = sf::Vector2f(third.x, second.y);
		mVerts[16 + 2].position = sf::Vector2f(third);
		mVerts[16 + 3].position = sf::Vector2f(second.x, third.y);

		// 5 right
		mVerts[20 + 0].position = sf::Vector2f(third.x, second.y);
		mVerts[20 + 1].position = sf::Vector2f(fourth.x, second.y);
		mVerts[20 + 2].position = sf::Vector2f(fourth.x, third.y);
		mVerts[20 + 3].position = sf::Vector2f(third);

		// 6 bottom left
		mVerts[24 + 0].position = sf::Vector2f(first.x, third.y);
		mVerts[24 + 1].position = sf::Vector2f(second.x, third.y);
		mVerts[24 + 2].position = sf::Vector2f(second.x, fourth.y);
		mVerts[24 + 3].position = sf::Vector2f(first.x, fourth.y);

		// 7 bottom center
		mVerts[28 + 0].position = sf::Vector2f(second.x, third.y);
		mVerts[28 + 1].position = sf::Vector2f(third);
		mVerts[28 + 2].position = sf::Vector2f(third.x, fourth.y);
		mVerts[28 + 3].position = sf::Vector2f(second.x, fourth.y);

		//  8 bottom right
		mVerts[32 + 0].position = sf::Vector2f(third);
		mVerts[32 + 1].position = sf::Vector2f(fourth.x, third.y);
		mVerts[32 + 2].position = sf::Vector2f(fourth);
		mVerts[32 + 3].position = sf::Vector2f(third.x, fourth.y);
	}
} // namespace