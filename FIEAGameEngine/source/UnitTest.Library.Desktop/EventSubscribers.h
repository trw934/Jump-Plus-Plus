#pragma once
#include "Event.h"
#include "EventQueue.h"
#include "Foo.h"

namespace UnitTestLibraryDesktop
{
	struct FooSubscriber final : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;

		bool WasNotified = false;
		int Data = 0;
	};

	struct FooCountSubscriber final : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;

		std::size_t Count = 0;
	};

	class EventEnqueueFoo final : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(EventEnqueueFoo, FieaGameEngine::RTTI)

	public:
		EventEnqueueFoo(EventQueue& eventQueue);
		EventQueue& Queue();
	private:
		EventQueue* _eventQueue;
	};

	struct EventEnqueueSubscriber : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;

		std::size_t Count = 0;
	};

	struct EventClearQueueSubscriber : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;
	};

	struct EventAddFooSubscriber : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;

		FooSubscriber* sub = nullptr;
	};

	struct EventUnsubscribeFooSubscriber : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;

		FooSubscriber* sub = nullptr;
	};

	struct EventUnsubscribeAll : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;
	};

}