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
		struct FunctionList
		{
			std::vector<std::function<void(GuiEvent&)>> functionList;
			std::vector<int> functionIDs;
		};

		CallbackManager();
		~CallbackManager();

		//template<class ...Ts>
		//void bindCallback(GuiEvent::EventType eventType, std::function<void(GuiEvent&, Ts...)> function, Ts... args)
		//{
		//	std::function<void(GuiEvent&)> func = std::bind(function, std::placeholders::_1, args...);
		//	mCallbacks[eventType].push_back(func);
		//}

		void bindCallback(GuiEvent::EventType eventType, std::function<void(GuiEvent&, void*)> function, void* args, int id)
		{
			std::function<void(GuiEvent&)> func = std::bind(function, std::placeholders::_1, args);
			auto& list = mCallbacks[eventType];
			list.functionList.push_back(func);
			list.functionIDs.push_back(id);
		}

		void unbind(GuiEvent::EventType eventType, int id)
		{
			auto& list = mCallbacks[eventType];

			auto& i = list.functionIDs.begin();
			for (int index = 0; i != list.functionIDs.end(); ++i, ++index)
			{
				if (*i == id)
				{
					list.functionList.erase(list.functionList.begin() + index);
					list.functionIDs.erase(i);
					return;
				}
			}
		}

		// Eliminates all callbacks associated with this event
		void unbindAll(GuiEvent::EventType eventType)
		{
			auto& list = mCallbacks[eventType];
			list.functionList.clear();
			list.functionIDs.clear();
		}

	protected:
		std::unordered_map<GuiEvent::EventType, FunctionList> mCallbacks;
	};
}
#endif