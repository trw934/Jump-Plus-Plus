#pragma once
#include "SDL2/SDL.h"
#include "Event.h"
#include "EventQueue.h"
#include "GameTime.h"

#include "InputEventPayload.h"

namespace FieaGameEngine
{
	class InputListener final
	{
	public:

		void SetEventQueue(FieaGameEngine::EventQueue& q);
		void Update(FieaGameEngine::GameTime& time);

	private:
		FieaGameEngine::EventQueue* queue{ nullptr };
	};

}
