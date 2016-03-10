#ifndef GUIPANEL_H
#define GUIPANEL_H

#include "Widget.h"


namespace gui
{
	class Panel : public Widget
	{
	public:
		using Ptr = std::shared_ptr<Panel>;

		Panel(const Widget::Ptr& parent, Gui* const gui, const std::string& name);
		~Panel();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	private:
		
	};
}//namespace

#endif
