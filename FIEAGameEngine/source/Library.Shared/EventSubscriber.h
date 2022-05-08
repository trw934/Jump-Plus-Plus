#pragma once
#include "RTTI.h"

namespace FieaGameEngine
{
	class EventPublisher;
	class EventSubscriber : public FieaGameEngine::RTTI 
	{
		RTTI_DECLARATIONS(EventSubscriber, FieaGameEngine::RTTI)
	public:
		/// <summary>
		/// Defaulted special members
		/// </summary>
		EventSubscriber() = default;
		virtual ~EventSubscriber() = default;
		EventSubscriber(const EventSubscriber& other) = default;
		EventSubscriber& operator=(const EventSubscriber& other) = default;
		EventSubscriber(EventSubscriber&& other) noexcept = default;
		EventSubscriber& operator=(EventSubscriber&& other) noexcept = default;

		/// <summary>
		/// Takes in a reference to an EventPublisher and performs certain actions based on the Event that notified it.
		/// </summary>
		/// <param name="publisher">Reference to the event passed in</param>
		virtual void Notify(const EventPublisher& publisher) = 0;
	};
}