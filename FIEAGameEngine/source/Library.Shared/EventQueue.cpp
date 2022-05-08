#include "pch.h"
#include "EventQueue.h"

namespace FieaGameEngine
{
	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> publisher, GameTime& gameTime, std::chrono::milliseconds delay)
	{
		if (_isUpdating)
		{
			_pendingEventQueue.PushBack(EventInfo(publisher, gameTime.CurrentTime(), delay));
		}
		else
		{
			_eventQueue.PushBack(EventInfo(publisher, gameTime.CurrentTime(), delay));
		}
	}

	void EventQueue::Send(std::shared_ptr<EventPublisher> publisher)
	{
		publisher.get()->Deliver();
	}

	void EventQueue::Update(GameTime& gameTime)
	{
		_isUpdating = true;
		auto iter = std::partition(_eventQueue.begin(), _eventQueue.end(), 
			[&gameTime](EventInfo eventInfo) {return gameTime.CurrentTime() <= eventInfo._timeExpired; });
		for (auto newIter = iter; newIter != _eventQueue.end(); ++newIter)
		{
			Send((*newIter)._event);
		}

		if (iter != _eventQueue.end())
		{
			_eventQueue.Remove(iter, _eventQueue.end());
		}
		_isUpdating = false;
		if (_pendingClear)
		{
			Clear();
		}
		for (auto pendingIter = _pendingEventQueue.begin(); pendingIter < _pendingEventQueue.end(); ++pendingIter)
		{
			_eventQueue.PushBack(*pendingIter);
		}
		_pendingEventQueue.Clear();
	}

	void EventQueue::Clear()
	{
		if (_isUpdating)
		{
			_pendingClear = true;
		}
		else
		{
			_pendingClear = false;
			_eventQueue.Clear();
		}
	}

	std::size_t EventQueue::Size() const
	{
		return _eventQueue.Size();
	}

	bool EventQueue::IsEmpty() const
	{
		return _eventQueue.Size() == 0;
	}
}