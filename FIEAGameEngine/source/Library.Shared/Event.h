#pragma once
#include "EventPublisher.h"

namespace FieaGameEngine
{
	enum class SubscriberCommands
	{
		Subscribe,
		Unsubscribe,
		UnsubscribeAll
	};

	template <typename MessageT>
	class Event : public FieaGameEngine::EventPublisher
	{
		RTTI_DECLARATIONS(Event<MessageT>, FieaGameEngine::EventPublisher)
	public:
		/// <summary>
		/// Constructs an event with the desired message
		/// </summary>
		/// <typeparam name="MessageT">Message associated with the event</typeparam>
		Event(MessageT& message);

		/// <summary>
		/// Constructs an event with the desired message
		/// </summary>
		/// <typeparam name="MessageT">Message associated with the event</typeparam>
		Event(MessageT&& message);

		/// <summary>
		/// Defaulted special members
		/// </summary>
		virtual ~Event() = default;
		Event(const Event& other) = default;
		Event& operator=(const Event& other) = default;
		Event(Event&& other) noexcept = default;
		Event& operator=(Event&& other) noexcept = default;

		/// <summary>
		/// Adds the EventSubscriber to the list of subscribers if it isn't already in the list.
		/// </summary>
		/// <param name="subscriber">Subscriber to add</param>
		static void Subscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Removes the event subscriber from the list of subscribers
		/// </summary>
		/// <param name="subscriber">Subscriber to remove</param>
		static void Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Removes all subscribers from the list of subscribers.
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Processes all pending requests to add or remove subscribers to the event. Must be called in order to see results from
		/// Subscribe, Unsubscribe, or UnsubscribeAll
		/// </summary>
		static void ProcessSubscriberCommands();

		/// <summary>
		/// Returns the number of subscribers currently subscribed to this event
		/// </summary>
		/// <returns>Number of subscribers in this event</returns>
		static std::size_t Size();

		/// <summary>
		/// Returns a reference to the message
		/// </summary>
		/// <returns>Reference to the message</returns>
		const MessageT& Message() const;
	private:
		inline static Vector<EventSubscriber*> _subscribers;
		inline static Vector<std::pair<SubscriberCommands, EventSubscriber*>> _pendingSubscriberCommands;
		MessageT _message;
	};
}

#include "Event.inl"