#ifndef CALLBACKMANAGER_H
#define CALLBACKMANAGER_H

#include "GuiEvent.h"

#include <functional>
#include <unordered_map>
#include <list>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

namespace gui
{
	class CallbackManager
	{
	public:
		using FunctionList = std::list<std::function<void(GuiEvent&)>>;

		CallbackManager();
		~CallbackManager();

		//template<class ...Ts>
		//void bindCallback(GuiEvent::EventType eventType, std::function<void(GuiEvent&, Ts...)> function, Ts... args)
		//{
		//	std::function<void(GuiEvent&)> func = std::bind(function, std::placeholders::_1, args...);
		//	mCallbacks[eventType].push_back(func);
		//}

		void bindCallback(GuiEvent::EventType eventType, std::function<void(GuiEvent&, void*)> function, void* args)
		{
			std::function<void(GuiEvent&)> func = std::bind(function, std::placeholders::_1, args);
			mCallbacks[eventType].push_back(func);
		}

		// Eliminates all callbacks associated with this event
		void unbindCallback(GuiEvent::EventType eventType)
		{
			mCallbacks[eventType].clear();
		}

	protected:
		std::unordered_map<GuiEvent::EventType, FunctionList> mCallbacks;
	};
}
#endif