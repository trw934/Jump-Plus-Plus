#include "pch.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher);

	EventPublisher::EventPublisher(Vector<EventSubscriber*>& subscribers) :
		_eventSubscribers{ &subscribers }
	{
	}

	void EventPublisher::Deliver()
	{
		for (auto& subscriber : *_eventSubscribers)
		{
			subscriber->Notify(*this);
		}
	}
}