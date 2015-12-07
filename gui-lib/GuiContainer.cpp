#include "GuiContainer.h"
#include "Gui.h"

namespace gui
{
	GuiContainer::GuiContainer(const Widget::Ptr& parent, Gui* const gui, const std::string& name) :
		Widget(parent, gui, name, sf::Vector2f(), gui->getWindow()->getSize(), true, true, false, false, false)
	{
	}


	GuiContainer::~GuiContainer()
	{
	}
}//namespace

