#include "Panel.h"
#include "Gui.h"

namespace gui
{
	Panel::Panel(const Widget::Ptr& parent, Gui* const gui, const std::string& name, const std::string& theme) :
		Widget(parent, gui, name, sf::Vector2f(), sf::Vector2u(50, 50), true, true, false, true, false)
	{
		loadTheme(theme);
	}


	Panel::~Panel()
	{
	}
}//namespace

