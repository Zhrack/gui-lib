#ifndef GUIPANEL_H
#define GUIPANEL_H

#include "Widget.h"


namespace guiSystem
{
	class Panel : public Widget
	{
	public:
		using Ptr = std::shared_ptr<Panel>;

		Panel(const Widget::Ptr& parent, Gui* const gui, const std::string& name);
		~Panel();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			// draw the vertex array
			target.draw(mRect, states);

			for (auto& widget : mChildWidgets)
			{
				target.draw(*widget, states);
			}
		}
	private:
		
	};
}//namespace

#endif
