#ifndef WIDGET_H
#define WIDGET_H

#include <vector>
#include <string>

#include "CallbackManager.h"

class Widget : public CallbackManager, public sf::Drawable, public sf::Transformable
{
public:
	Widget();
	~Widget();

	void show()
	{
		mEnabled = true;
		mVisible = true;
	}
	void hide()
	{
		mEnabled = false;
		mVisible = false;
	}

	void enable(){ mEnabled = true; }
	void disable(){ mEnabled = false; }
	
	void focus() { mFocused = true; }
	void unfocus(){ mFocused = false; }

	//getters
	bool isEnabled() const { return mEnabled; }
	bool isVisible() const { return mVisible; }
	bool isFocused() const { return mFocused; }

	Widget* getParent() const { return mParent; }

	void handleEvent(sf::Event& event);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) = 0;


protected:
	std::vector<std::string> mWidgetNames;
	std::vector<Widget*> mChildWidgets;

	Widget* mParent;

	//accepts events?
	bool mEnabled;
	//is rendered?
	bool mVisible;
	//is focused?
	bool mFocused;

};


#endif