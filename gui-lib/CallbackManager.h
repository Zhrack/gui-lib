#ifndef CALLBACKMANAGER_H
#define CALLBACKMANAGER_H

#include "GuiEvent.h"

#include <functional>
#include <unordered_map>
#include <list>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

namespace guiSystem
{
	class CallbackManager
	{
	public:
		using FunctionList = std::list<std::function<void(GuiEvent&)>>;

		CallbackManager();
		~CallbackManager();

	protected:
		std::unordered_map<GuiEvent::EventType, FunctionList> mCallbacks;
	};
}
#endif