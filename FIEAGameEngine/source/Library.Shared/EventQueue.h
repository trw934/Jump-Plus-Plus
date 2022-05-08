#pragma once
#include "Event.h"
#include "GameTime.h"
#include <algorithm>

namespace FieaGameEngine
{
	class EventQueue final
	{
	public:
		/// <summary>
		/// Adds the event to the queue with the specified delay
		/// </summary>
		/// <param name="publisher">The event to add to the queue</param>
		/// <param name="gameTime">The time that the event is added to the queue</param>
		/// <param name="delay">The delay before the event expires</param>
		void Enqueue(std::shared_ptr<EventPublisher> publisher, GameTime& gameTime, std::chrono::milliseconds delay = static_cast<std::chrono::milliseconds>(0));

		/// <summary>
		/// Instantly delivers the desired event
		/// </summary>
		/// <param name="publisher">Event to be delivered</param>
		void Send(std::shared_ptr<EventPublisher> publisher);

		/// <summary>
		/// Iterates through event queue and delivers any expired events.
		/// </summary>
		/// <param name="gameTime">GameTime to use as reference for updating</param>
		void Update(GameTime& gameTime);

		/// <summary>
		/// Clears the event queue.
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns the size of the event queue.
		/// </summary>
		/// <returns>Size of the event queue.</returns>
		std::size_t Size() const;

		/// <summary>
		/// Checks if the queue is empty or not
		/// </summary>
		/// <returns>True if the queue is empty, false otherwise</returns>
		bool IsEmpty() const;

	private:
		struct EventInfo {
			EventInfo(std::shared_ptr<EventPublisher> publisher, const std::chrono::high_resolution_clock::time_point& gameTime, std::chrono::milliseconds delay = static_cast<std::chrono::milliseconds>(0)) :
				_event{ publisher }, _timeExpired{ gameTime + delay }
			{
			}
			bool operator==(const EventInfo& other)
			{
				return (_event == other._event && _timeExpired == other._timeExpired);
			}

			std::shared_ptr<EventPublisher> _event;
			std::chrono::high_resolution_clock::time_point _timeExpired;
		};
		Vector<EventInfo> _eventQueue;
		Vector<EventInfo> _pendingEventQueue;
		bool _isUpdating = false;
		bool _pendingClear = false;
	};
}