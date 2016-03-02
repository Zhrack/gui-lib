#include "Callbacks.h"
#include <iostream>

#include "Gui.h"

#include "ScrollBar.h"

#define USEDEBUG
#ifdef USEDEBUG
#define Debug(x) std::cout << x
#else
#define Debug(x) 
#endif 

namespace gui
{
	////////////// BORDERWIDGET
	void BorderWidgetCallbacks::OnMouseEntered(gui::GuiEvent& event, void* args)
	{
		BorderWidget* widget = static_cast<BorderWidget*>(args);
		// Set Hover state
		widget->toHoverButtonState();
	}

	void BorderWidgetCallbacks::OnMouseLeft(gui::GuiEvent& event, void* args)
	{
		BorderWidget* widget = static_cast<BorderWidget*>(args);
		// Set Normal state
		widget->toNormalButtonState();
	}

	void BorderWidgetCallbacks::OnMouseButtonDown(gui::GuiEvent& event, void* args)
	{
		BorderWidget* widget = static_cast<BorderWidget*>(args);
		// Set Down state
		widget->toDownButtonState();
	}

	void BorderWidgetCallbacks::OnMouseButtonUp(gui::GuiEvent& event, void* args)
	{
		BorderWidget* widget = static_cast<BorderWidget*>(args);
		// Set Hover state
		widget->toHoverButtonState();
	}

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
		sf::Vector2i oldMousePos = widget->getGui()->getOldMousePosition();

		sf::Vector2f movement = pos - sf::Vector2f(oldMousePos);

		if (widget->isVertical())
		{
			widget->setValue(widget->getValue() + movement.y);
		}
		else // Horizontal
		{
			widget->setValue(widget->getValue() + movement.x);
		}
	}

} // namespace