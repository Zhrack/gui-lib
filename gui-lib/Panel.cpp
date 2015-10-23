#include "Panel.h"

namespace guiSystem
{
	Panel::Panel(const Widget::Ptr& parent, Gui* const gui, const std::string& name) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(50, 50), true, true, false, true, false)
	{

	}


	Panel::~Panel()
	{
	}
}//namespace

