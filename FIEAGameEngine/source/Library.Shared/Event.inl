#include "Event.h"

namespace FieaGameEngine
{
	template <typename MessageT>
	RTTI_DEFINITIONS(Event<MessageT>)

	template <typename MessageT>
	Event<MessageT>::Event(MessageT& message) :
		EventPublisher(_subscribers), _message{ message }
	{
	}

	template <typename MessageT>
	Event<MessageT>::Event(MessageT&& message) :
		EventPublisher(_subscribers), _message{ message }
	{
	}

	template <typename MessageT>
	void Event<MessageT>::Subscribe(EventSubscriber& subscriber)
	{
		_pendingSubscriberCommands.PushBack({ SubscriberCommands::Subscribe, &subscriber });
	}

	template <typename MessageT>
	void Event<MessageT>::Unsubscribe(EventSubscriber& subscriber)
	{
		_pendingSubscriberCommands.PushBack({ SubscriberCommands::Unsubscribe, &subscriber });
	}

	template <typename MessageT>
	void Event<MessageT>::UnsubscribeAll()
	{
		_pendingSubscriberCommands.PushBack({ SubscriberCommands::UnsubscribeAll, nullptr });
	}

	template <typename MessageT>
	void Event<MessageT>::ProcessSubscriberCommands()
	{
		for (auto& [function, subscriber] : _pendingSubscriberCommands)
		{
			switch (function)
			{
			case SubscriberCommands::Subscribe:
				if (_subscribers.Find(subscriber) == _subscribers.end())
				{
					_subscribers.PushBack(subscriber);
				}
				break;
			case SubscriberCommands::Unsubscribe:
				_subscribers.Remove(subscriber);
				break;
			case SubscriberCommands::UnsubscribeAll:
				_subscribers.Clear();
				break;
			default:
				break;
			}
		}
		_pendingSubscriberCommands.Clear();
	}

	template <typename MessageT>
	std::size_t Event<MessageT>::Size()
	{
		return _subscribers.Size();
	}

	template <typename MessageT>
	inline const typename MessageT& Event<MessageT>::Message() const
	{
		return _message;
	}
}