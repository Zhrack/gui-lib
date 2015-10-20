#include <SFML/Window.hpp>

#include "Gui.h"
#include <iostream>

void test(guiSystem::GuiEvent& event, int i, float k)
{
	std::cout << "test function" << std::endl;
	std::cout << i++ << " " << k << std::endl;
}

int main()
{
	sf::RenderWindow* window(new sf::RenderWindow(sf::VideoMode(800, 600), "My window"));

	guiSystem::Gui gui(window);
	guiSystem::Panel::Ptr panel = gui.createPanel("panel");

	panel->setPosition(sf::Vector2f(50, 50));
	panel->setDraggable(true);
	int i = 0;
	std::function<void(guiSystem::GuiEvent&, int, float)> function = test;
	panel->bindCallback(guiSystem::GuiEvent::MouseButtonPressed, function, i, 2.5f);
	// run the program as long as the window is open
	while (window->isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window->pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window->close();

			if (gui.handleEvent(event) == false)
			{
				// Game input here
			}
		}

		gui.draw();

		window->display();
	}

	delete window;
	return 0;
}