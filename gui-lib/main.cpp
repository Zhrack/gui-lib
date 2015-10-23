#include <SFML/Window.hpp>

#include "Gui.h"
#include <iostream>

struct testStruct
{
	guiSystem::Panel::Ptr panel;
};

void test(guiSystem::GuiEvent& event, void* args)
{
	testStruct* str = static_cast<testStruct*>(args);

	std::cout << "test " << str->panel->getName() << std::endl;
	std::cout << "focus: " << str->panel->mMainGui->getFocusedWidget()->getName() << std::endl;
	std::cout << "Position: " << str->panel->getGlobalPosition().x << " " << str->panel->getGlobalPosition().y << std::endl << std::endl;
}

int main()
{
	sf::RenderWindow* window(new sf::RenderWindow(sf::VideoMode(800, 600), "My window"));

	guiSystem::Gui gui(window);
	guiSystem::Panel::Ptr panel = gui.createPanel("panel");
	panel->getShape().setSize(sf::Vector2f(120, 120));
	//panel->setPosition(sf::Vector2f(50, 50));
	
	panel->setDraggable(true);
	panel->getShape().setFillColor(sf::Color(200, 50, 50));

	guiSystem::Panel::Ptr childPanel = gui.createPanel(panel, "childPanel");
	childPanel->getShape().setSize(sf::Vector2f(60, 30));
	childPanel->setDraggable(true);
	childPanel->getShape().setFillColor(sf::Color::Blue);
	//childPanel->setPosition(sf::Vector2f(60, 60));

	guiSystem::Panel::Ptr childPanel2 = gui.createPanel(childPanel, "childPanel2");
	childPanel2->getShape().setSize(sf::Vector2f(30, 20));
	childPanel2->setDraggable(true);
	
	testStruct str;
	str.panel = panel;
	std::function<void(guiSystem::GuiEvent& event, void*)> function = test;
	panel->bindCallback(guiSystem::GuiEvent::MouseButtonPressed, function, &str);

	testStruct str1;
	str1.panel = childPanel;
	childPanel->bindCallback(guiSystem::GuiEvent::MouseButtonPressed, function, &str1);

	testStruct str2;
	str2.panel = childPanel2;
	childPanel2->bindCallback(guiSystem::GuiEvent::MouseButtonPressed, function, &str2);

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
		window->clear();

		gui.draw();

		window->display();
	}

	delete window;
	return 0;
}