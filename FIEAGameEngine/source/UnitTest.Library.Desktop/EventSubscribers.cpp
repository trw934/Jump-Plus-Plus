#include "pch.h"
#include "EventSubscribers.h"

using namespace std;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	void FooSubscriber::Notify(const EventPublisher& event)
	{
		const Event<Foo>* e = event.As<Event<Foo>>();
		if (e != nullptr)
		{
			const Foo& foo = e->Message();
			Data = foo.Data();
		}
		WasNotified = true;
	}

	void FooCountSubscriber::Notify(const EventPublisher&)
	{
		++Count;
	}

	RTTI_DEFINITIONS(EventEnqueueFoo);

	EventEnqueueFoo::EventEnqueueFoo(EventQueue& eventQueue) :
		_eventQueue{ &eventQueue }
	{
	}

	EventQueue& EventEnqueueFoo::Queue()
	{
		return *_eventQueue;
	}

	void EventEnqueueSubscriber::Notify(const EventPublisher& event)
	{
		++Count;

		const Event<EventEnqueueFoo>* e = event.As<Event<EventEnqueueFoo>>();
		if (e != nullptr)
		{
			EventEnqueueFoo message = e->Message();
			EventQueue& queue = message.Queue();

			GameTime gameTime;
			EventEnqueueFoo eventEnqueueFoo(queue);
			auto newEvent = make_shared<Event<EventEnqueueFoo>>(eventEnqueueFoo);
			queue.Enqueue(newEvent, gameTime);
		}
	}

	void EventClearQueueSubscriber::Notify(const EventPublisher& event)
	{

		const Event<EventEnqueueFoo>* e = event.As<Event<EventEnqueueFoo>>();
		if (e != nullptr)
		{
			EventEnqueueFoo message = e->Message();
			EventQueue& queue = message.Queue();
			queue.Clear();
		}
	}

	void EventAddFooSubscriber::Notify(const EventPublisher& event)
	{
		const Event<Foo>* e = event.As<Event<Foo>>();
		if (e != nullptr && sub != nullptr)
		{
			e->Subscribe(*sub);
		}
	}

	void EventUnsubscribeFooSubscriber::Notify(const EventPublisher& event)
	{
		const Event<Foo>* e = event.As<Event<Foo>>();
		if (e != nullptr && sub != nullptr)
		{
			e->Unsubscribe(*sub);
		}
	}

	void EventUnsubscribeAll::Notify(const EventPublisher& event)
	{
		const Event<Foo>* e = event.As<Event<Foo>>();
		if (e != nullptr)
		{
			e->UnsubscribeAll();
		}
	}

}