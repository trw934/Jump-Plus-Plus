#pragma once
#include "Event.h"
#include "EventSubscriber.h"
#include "EventQueue.h"

class PlayerJumpEventSubscriber final : public FieaGameEngine::EventSubscriber
{
	void Notify(const FieaGameEngine::EventPublisher& event) override;

	bool wasNotified{ false };
};