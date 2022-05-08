#pragma once
#include "RTTI.h"
#include "EventSubscriber.h"
#include "Vector.h"

namespace FieaGameEngine
{
	class EventPublisher : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, FieaGameEngine::RTTI);
	public:
		/// <summary>
		/// Defaulted special members
		/// </summary>
		virtual ~EventPublisher() = default;
		EventPublisher(const EventPublisher& other) = default;
		EventPublisher& operator=(const EventPublisher& other) = default;
		EventPublisher(EventPublisher&& other) noexcept = default;
		EventPublisher& operator=(EventPublisher&& other) noexcept = default;

		/// <summary>
		/// Notifies all subscribers that are subscribed to this event
		/// </summary>
		void Deliver();
	protected:
		EventPublisher(Vector<EventSubscriber*>& subscribers);
		Vector<EventSubscriber*>* _eventSubscribers;
	};
}