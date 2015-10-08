#ifndef CALLBACKMANAGER_H
#define CALLBACKMANAGER_H

#include <functional>
#include <unordered_map>
#include <list>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

class CallbackManager
{
public:
	using FunctionList = std::list<std::function<void(sf::Event&)>>;

	CallbackManager();
	~CallbackManager();

protected:
	std::unordered_map<sf::Event::EventType, FunctionList> mCallbacks;
};

#endif