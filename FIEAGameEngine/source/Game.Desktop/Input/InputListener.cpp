#include "pch.h"
#include "InputListener.h"
namespace FieaGameEngine
{

    void InputListener::SetEventQueue(FieaGameEngine::EventQueue& q)
    {
        queue = &q;
    }

    void InputListener::Update(FieaGameEngine::GameTime& time)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                exit(0);
                break;
            }

            case SDL_KEYDOWN:
            {
                
                InputDownEventPayload payload = { event.key.keysym.scancode == SDL_SCANCODE_A,
                            event.key.keysym.scancode == SDL_SCANCODE_D,
                            event.key.keysym.scancode == SDL_SCANCODE_F };
                std::shared_ptr<FieaGameEngine::Event<InputDownEventPayload>> keyDownEvent = std::make_shared<FieaGameEngine::Event<InputDownEventPayload>>(payload);

                queue->Enqueue(keyDownEvent, time);
                break;
            }

            case SDL_KEYUP:
            {
              

                InputUpEventPayload payload = { event.key.keysym.scancode == SDL_SCANCODE_A,
                            event.key.keysym.scancode == SDL_SCANCODE_D,
                            event.key.keysym.scancode == SDL_SCANCODE_F };
                std::shared_ptr<FieaGameEngine::Event<InputUpEventPayload>> keyUpEvent = std::make_shared<FieaGameEngine::Event<InputUpEventPayload>>(payload);

                queue->Enqueue(keyUpEvent, time);
                break;
            }

            default:
                break;
            }
        }
    }
}