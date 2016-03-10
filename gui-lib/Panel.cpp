#include "Panel.h"
#include "Gui.h"

namespace gui
{
	Panel::Panel(const Widget::Ptr& parent, Gui* const gui, const std::string& name) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(50, 50), true, true, false, true, false)
	{
	}


	Panel::~Panel()
	{
	}

	void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		if (isVisible())
		{
			target.draw(mRect, states);

			mMainGui->setScissor(mRect.getGlobalBounds());

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}

			mMainGui->resetScissor();
		}
	}

}//namespace

