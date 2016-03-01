#include "Callbacks.h"
#include <iostream>

#include "ScrollBar.h"

#define USEDEBUG
#ifdef USEDEBUG
#define Debug(x) std::cout << x
#else
#define Debug(x) 
#endif 

namespace gui
{
	////////////// SCROLLBAR

	void ScrollBarCallbacks::OnUpLeftArrowClick(gui::GuiEvent& event, void* args)
	{
		ScrollBar* widget = static_cast<ScrollBar*>(args);
		int value = widget->getValue() - widget->getScrollAmount();
		value = value < 0 ? 0 : value;
		widget->setValue(value);
	}

	void ScrollBarCallbacks::OnDownRightArrowClick(gui::GuiEvent& event, void* args)
	{
		ScrollBar* widget = static_cast<ScrollBar*>(args);
		widget->setValue(widget->getValue() + widget->getScrollAmount());
	}

	void ScrollBarCallbacks::OnThumbDrag(gui::GuiEvent& event, void* args)
	{
		ScrollBar* widget = static_cast<ScrollBar*>(args);
		sf::Vector2f pos(event.mouseDrag.x, event.mouseDrag.y);

		Debug(widget->getName() << ": drag" << std::endl);

		widget->setValue
			(
			widget->isVertical()
			? (pos - widget->mThumb->getGlobalPosition()).y
			: (pos - widget->mThumb->getGlobalPosition()).x
			);
	}

	////////////// END SCROLLBAR

} // namespace