#include <SFML/Window.hpp>

#include "Gui.h"
#include <iostream>

struct testStruct
{
	gui::Panel::Ptr panel;
};

void test(gui::GuiEvent& event, void* args)
{
	testStruct* str = static_cast<testStruct*>(args);

	std::cout << "test " << str->panel->getName() << std::endl;
	std::cout << "focus: " << str->panel->getGui()->getFocusedWidget()->getName() << std::endl;
	std::cout << "Position: " << str->panel->getGlobalPosition().x << " " << str->panel->getGlobalPosition().y << std::endl << std::endl;
}

int main()
{
	sf::RenderWindow* window(new sf::RenderWindow(sf::VideoMode(800, 600), "My window"));

	gui::Gui gui(window);
	gui::Panel::Ptr panel = gui.createPanel("panel");
	panel->getShape().setSize(sf::Vector2f(120, 120));
	//panel->setPosition(sf::Vector2f(50, 50));
	
	panel->setDraggable(true);
	panel->getShape().setFillColor(sf::Color(200, 50, 50));

	gui::Panel::Ptr childPanel = gui.createPanel(panel, "childPanel");
	childPanel->getShape().setSize(sf::Vector2f(60, 30));
	//childPanel->setDraggable(true);
	childPanel->getShape().setFillColor(sf::Color::Blue);
	//childPanel->disable();
	//childPanel->setPosition(sf::Vector2f(60, 60));

	gui::Panel::Ptr childPanel2 = gui.createPanel(childPanel, "childPanel2");
	childPanel2->getShape().setSize(sf::Vector2f(30, 20));
	childPanel2->setDraggable(true);
	
	testStruct str;
	str.panel = panel;
	std::function<void(gui::GuiEvent& event, void*)> function = test;
	panel->bindCallback(gui::GuiEvent::MouseButtonPressed, function, &str);

	testStruct str1;
	str1.panel = childPanel;
	childPanel->bindCallback(gui::GuiEvent::MouseButtonPressed, function, &str1);

	testStruct str2;
	str2.panel = childPanel2;
	childPanel2->bindCallback(gui::GuiEvent::MouseButtonPressed, function, &str2);

	gui::Label::Ptr text = gui.createLabel("text");
	text->setText("Hello World!");
	text->setDraggable(true);

	gui::TextButton::Ptr button = gui.createTextButton("button", "BIIIIIIIIIIIIIIIIIIIIIIIIIIIIG TEXT");
	button->setDraggable(true);

	// run the program as long as the window is open
	while (window->isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		bool inputProcessed = false;
		while (window->pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window->close();

			inputProcessed = gui.handleEvent(event);
		}

		gui.update();

		if (!inputProcessed)
		{
			// Game input here
		}

		window->clear();

		gui.draw();

		window->display();
	}

	delete window;
	return 0;
}