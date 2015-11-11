#include "Button.h"

#include <assert.h>

namespace gui
{
	Button::Button(const Widget::Ptr& parent, Gui* const gui, const std::string& name, sf::Vector2f* points, const std::string& text) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(50, 50), true, true, false, true, false),
		mText(new TextWidget(static_cast<Widget::Ptr>(this), gui, "")),
		mVerts(sf::PrimitiveType::Quads, 9 * 4)
	{
		mText->setText(text);

		updateNinePatchPoints(points);
	}


	Button::~Button()
	{
	}

	//	Points must be an array of size 4, with coords of 4 points
	//	indicated by *, the rest is calculated from them.
	//	*--+--+--+
	//	|  |  |  |
	//	+--*--+--+
	//	|  |  |  |
	//	+--+--*--+
	//	|  |  |  |
	//	+--+--+--*
	void Button::updateNinePatchPoints(sf::Vector2f* points)
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

		// Update verts position
		updateVertsPosition();
	}

	void Button::updateVertsPosition()
	{
		sf::FloatRect rect = mRect.getGlobalBounds();
		sf::Vector2f first = sf::Vector2f(rect.top, rect.left);
		sf::Vector2f second = sf::Vector2f(rect.top, rect.left);
		sf::Vector2f third = sf::Vector2f(rect.top, rect.left);
		sf::Vector2f fourth = sf::Vector2f(rect.top + rect.width, rect.left + rect.height);

		// 0 top left
		mVerts[0 + 0].position = sf::Vector2f(first);
		mVerts[0 + 1].position = sf::Vector2f(second.x, first.y);
		mVerts[0 + 2].position = sf::Vector2f(second);
		mVerts[0 + 3].position = sf::Vector2f(first.x, second.y);

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
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isEnabled() && isVisible())
		{
			target.draw(mRect, states); // debug
			states.texture = mTexture;
			target.draw(mVerts, states);
			states.texture = nullptr;
			target.draw(*mText, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	}
} // namespace