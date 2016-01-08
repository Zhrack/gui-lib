#include <SFML/Window.hpp>

#include "Gui.h"
#include <iostream>

struct testStruct
{
	gui::Panel::Ptr panel;
	gui::Gui* gui;
	gui::Label::Ptr newChild;
};

void test(gui::GuiEvent& event, void* args)
{
	testStruct* str = static_cast<testStruct*>(args);

	std::cout << "test " << str->panel->getName() << std::endl;
	std::cout << "focus: " << str->panel->getGui()->getFocusedWidget()->getName() << std::endl;
	std::cout << "Position: " << str->panel->getGlobalPosition().x << " " << str->panel->getGlobalPosition().y << std::endl << std::endl;
}

void removeChild(gui::GuiEvent& event, void* args)
{
	gui::Label::Ptr* labelPtr = static_cast<gui::Label::Ptr*>(args);
	gui::Label::Ptr label = *labelPtr;

	label->getParent()->removeChild(label);
}

void resizeImage(gui::GuiEvent& event, void* args)
{
	gui::ImageButton::Ptr* buttonPtr = static_cast<gui::ImageButton::Ptr*>(args);
	gui::ImageButton::Ptr button = *buttonPtr;

	button->setSize(400, 400);
}

int main()
{
	sf::RenderWindow* window(new sf::RenderWindow(sf::VideoMode(800, 600), "My window"));

	gui::Gui gui(window);
	gui::Panel::Ptr panel = gui.createPanel("panel");
	panel->setSize(sf::Vector2f(300, 300));
	//panel->setPosition(sf::Vector2f(50, 50));
	
	panel->setDraggable(true);
	panel->getShape().setFillColor(sf::Color(200, 50, 50));

	gui::Panel::Ptr childPanel = gui.createPanel(panel, "childPanel");
	childPanel->setSize(sf::Vector2f(250, 100));
	childPanel->setDraggable(true);
	childPanel->getShape().setFillColor(sf::Color::Blue);

	gui::Panel::Ptr childPanel2 = gui.createPanel(childPanel, "childPanel2");
	childPanel2->setSize(sf::Vector2f(150, 70));
	childPanel2->setDraggable(true);
	
	testStruct str;
	str.panel = panel;
	str.gui = &gui;
	str.newChild = nullptr;
	std::function<void(gui::GuiEvent& event, void*)> function = test;
	panel->bindCallback(gui::GuiEvent::MouseButtonPressed, function, &str, 0);

	gui::Label::Ptr text = gui.createLabel(panel, "text");
	
	text->setWrapStyle(gui::Label::TextWrap::Horizontal, 200);
	text->setText("Hello Wo rldaaaab aaaaaac aaad aaaae aaaaaaaf aaaaaaag aaaaaaaah aaaaai aaaaaaal aaaaam!");
	text->setDraggable(true);
	text->setGlobalPosition(sf::Vector2f(100, 200));

	gui::TextButton::Ptr button = gui.createTextButton("button", "Remove Label");
	button->setDraggable(true);
	button->setTextWrapStyle(gui::Label::TextWrap::Horizontal, 200);
	button->setGlobalPosition(sf::Vector2f(400, 400));
	function = removeChild;
	button->bindCallback(gui::GuiEvent::MouseButtonPressed, function, &text, 0);

	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile("themes/defaultTheme.png");
	gui::ImageButton::Ptr imageButton = gui.createImageButton("imageButton", texture, sf::IntRect(0, 0, 400, 481));
	imageButton->setSize(200, 200);
	imageButton->setDraggable(true);

	gui::ScrollBar::Ptr scrollBar = gui.createScrollBar("scrollBar");


	//gui::ChildWindow::Ptr childWindow = gui.createChildWindow("childWindow", "My Window");
	//childWindow->setSize(400, 400);
	//childWindow->setDraggable(true);

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
			// Game logic here
		}

		window->clear();

		gui.draw();

		window->display();
	}

	delete window;
	return 0;
}