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

		void draw(sf::RenderTarget& target, sf::RenderStates states, bool recursive) const
		{
			

			if (recursive)
			{
				for (auto& widget : mChildWidgets)
				{
					widget->draw(target, states);
				}
			}
		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			// apply the entity's transform -- combine it with the one that was passed by the caller
			states.transform *= mRect.getTransform(); // getTransform() is defined by sf::Transformable

			// draw the vertex array
			target.draw(mRect, states);

			draw(target, states, true);
		}
	private:
		
	};
}//namespace

#endif
