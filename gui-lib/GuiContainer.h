#ifndef GUICONTAINER_H
#define GUICONTAINER_H

#include "Widget.h"


namespace guiSystem
{
	// A simple class, which contains all the GUI
	class GuiContainer : public Widget
	{
	public:
		using Ptr = std::shared_ptr<GuiContainer>;

		GuiContainer(const Widget::Ptr& parent, Gui* const gui, const std::string& name);
		~GuiContainer();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget);
			}
		}
	private:

	};
}//namespace

#endif
