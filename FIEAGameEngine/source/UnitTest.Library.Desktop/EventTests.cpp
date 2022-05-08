#include "pch.h"
#include "ToStringSpecializations.h"
#include "GameTime.h"
#include "GameState.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTestLibraryDesktop;
using namespace std;
using namespace std::string_literals;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(RTTITest)
		{
			Foo foo(69);
			Event<Foo> event(foo);

			RTTI* rtti = &event;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(rtti->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			Event<Foo>* e = rtti->As<Event<Foo>>();
			Assert::IsNotNull(e);
			Assert::AreEqual(&event, e);

			EventPublisher* ep = rtti->As<EventPublisher>();
			Assert::IsNotNull(ep);
			Assert::AreEqual(static_cast<EventPublisher*>(&event), ep);

		}

		TEST_METHOD(TestEventDeliver)
		{
			Foo foo(69);
			FooSubscriber fooSubscriber;
			Event<Foo> event(foo);
			Assert::AreEqual(foo, event.Message());

			Assert::IsFalse(fooSubscriber.WasNotified);
			event.Deliver();
			Assert::IsFalse(fooSubscriber.WasNotified);

			Event<Foo>::Subscribe(fooSubscriber);
			Event<Foo>::ProcessSubscriberCommands();
			event.Deliver();
			Assert::IsTrue(fooSubscriber.WasNotified);
			Assert::AreEqual(event.Message().Data(), fooSubscriber.Data);
			Event<Foo>::UnsubscribeAll();
			Event<Foo>::ProcessSubscriberCommands();
		}



		TEST_METHOD(EventQueueEnqueue)
		{
			GameTime gameTime;
			Foo foo(69);
			FooSubscriber fooSubscriber;
			EventQueue eventQueue;

			shared_ptr<Event<Foo>> event = make_shared<Event<Foo>>(foo);
			Assert::AreEqual(event->Message(), foo);

			Assert::AreEqual(size_t(0), eventQueue.Size());
			eventQueue.Enqueue(event, gameTime);
			Assert::AreEqual(size_t(1), eventQueue.Size());

			Event<Foo>::Subscribe(fooSubscriber);
			Event<Foo>::ProcessSubscriberCommands();
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1s));
			Assert::IsFalse(fooSubscriber.WasNotified);
			eventQueue.Update(gameTime);

			// Verifies that the message was removed from queue after delivery
			fooSubscriber.WasNotified = false;
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			Assert::AreEqual(size_t(0), eventQueue.Size());

			// Test delay
			gameTime.SetCurrentTime(high_resolution_clock::time_point());
			eventQueue.Enqueue(event, gameTime, 1s);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(500ms));
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1s));
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			gameTime.SetCurrentTime(high_resolution_clock::time_point(2s));
			eventQueue.Update(gameTime);
			Assert::IsTrue(fooSubscriber.WasNotified);

			Assert::IsTrue(eventQueue.IsEmpty());
			Event<Foo>::UnsubscribeAll();
			Event<Foo>::ProcessSubscriberCommands();
		
		}

		TEST_METHOD(EventQueueMultipleEvents)
		{
			FooCountSubscriber subscriber;
			Event<Foo>::Subscribe(subscriber);
			Event<Foo>::ProcessSubscriberCommands();

			shared_ptr<Event<Foo>> event1 = make_shared<Event<Foo>>(Foo{ 69 });
			shared_ptr<Event<Foo>> event2 = make_shared<Event<Foo>>(Foo{ 69 });
			shared_ptr<Event<Foo>> event3 = make_shared<Event<Foo>>(Foo{ 69 });
			shared_ptr<Event<Foo>> event4 = make_shared<Event<Foo>>(Foo{ 69 });

			GameTime gameTime;
			EventQueue eventQueue;
			eventQueue.Enqueue(event1, gameTime, milliseconds(750));
			eventQueue.Enqueue(event2, gameTime);
			eventQueue.Enqueue(event3, gameTime);
			eventQueue.Enqueue(event4, gameTime, seconds(1));
			gameTime.SetCurrentTime(high_resolution_clock::time_point(500ms));
			eventQueue.Update(gameTime);
			Assert::AreEqual(size_t(2), subscriber.Count);

			gameTime.SetCurrentTime(high_resolution_clock::time_point(1500ms));
			eventQueue.Update(gameTime);
			Assert::AreEqual(size_t(4), subscriber.Count);

			Assert::IsTrue(eventQueue.IsEmpty());
			Event<Foo>::UnsubscribeAll();
			Event<Foo>::ProcessSubscriberCommands();
		}

		TEST_METHOD(CopySemantics)
		{
			{
				//Test copy constructor
				Event<Foo> event(Foo{69});
				Event<Foo> anotherEvent(event);
				Assert::AreNotSame(event.Message(), anotherEvent.Message());
				Assert::AreEqual(event.Message(), anotherEvent.Message());
			}
			{
				//Test copy assignment operator
				Event<Foo> event(Foo{69});
				Event<Foo> anotherEvent(Foo{ 70 });
				anotherEvent = event;
				Assert::AreNotSame(event.Message(), anotherEvent.Message());
				Assert::AreEqual(event.Message(), anotherEvent.Message());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			{
				//Test move constructor
				Event<Foo> event(Foo{69});
				Event<Foo> anotherEvent(move(event));
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::IsFalse(&event.Message() == &anotherEvent.Message());
#pragma warning(pop)
			}
			{
				//Test move assignment operator
				Event<Foo> event(Foo{69});
				Event<Foo> anotherEvent(Foo{ 70 });
				anotherEvent = move(event);
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::IsFalse(&event.Message() == &anotherEvent.Message());
#pragma warning(pop)
			}
		}

		TEST_METHOD(EventQueueClear)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			Assert::AreEqual(size_t(0), eventQueue.Size());

			const size_t eventCount = 4;
			for (size_t i = 0; i < eventCount; ++i)
			{
				eventQueue.Enqueue(make_shared<Event<Foo>>(Foo{ 69 }), gameTime);
			}

			Assert::AreEqual(eventCount, eventQueue.Size());
			eventQueue.Clear();
			Assert::AreEqual(size_t(0), eventQueue.Size());
		}

		TEST_METHOD(EventSubscriberThatEnqueuesEvents)
		{
			GameTime gameTime;
			Foo foo(69);
			EventEnqueueSubscriber subscriber;
			EventQueue eventQueue;

			shared_ptr<Event<EventEnqueueFoo>> event = make_shared<Event<EventEnqueueFoo>>(EventEnqueueFoo(eventQueue));
			eventQueue.Enqueue(event, gameTime);

			Event<EventEnqueueFoo>::Subscribe(subscriber);
			Event<EventEnqueueFoo>::ProcessSubscriberCommands();
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1000ms));
			Assert::AreEqual(size_t(0), subscriber.Count);
			Assert::AreEqual(size_t(1), eventQueue.Size());
			eventQueue.Update(gameTime);
			Assert::AreEqual(size_t(1), subscriber.Count);
			Assert::AreEqual(size_t(1), eventQueue.Size());

			eventQueue.Update(gameTime);
			Assert::AreEqual(size_t(2), subscriber.Count);
			Assert::AreEqual(size_t(1), eventQueue.Size());

			Event<EventEnqueueFoo>::UnsubscribeAll();
			Event<EventEnqueueFoo>::ProcessSubscriberCommands();
		}

		TEST_METHOD(EventPendingClearQueue)
		{
			GameTime gameTime;
			Foo foo(69);
			FooCountSubscriber countSubscriber;
			EventClearQueueSubscriber clearSubscriber;
			EventQueue eventQueue;

			shared_ptr<Event<Foo>> event1 = make_shared<Event<Foo>>(Foo{ 69 });
			shared_ptr<Event<Foo>> event2 = make_shared<Event<Foo>>(Foo{ 69 });
			shared_ptr<Event<Foo>> event3 = make_shared<Event<Foo>>(Foo{ 69 });
			shared_ptr<Event<Foo>> event4 = make_shared<Event<Foo>>(Foo{ 69 });

			shared_ptr<Event<EventEnqueueFoo>> event = make_shared<Event<EventEnqueueFoo>>(EventEnqueueFoo(eventQueue));
			eventQueue.Enqueue(event, gameTime);

			eventQueue.Enqueue(event1, gameTime, milliseconds(750));
			eventQueue.Enqueue(event2, gameTime, 2000ms);
			eventQueue.Enqueue(event3, gameTime, 2000ms);
			eventQueue.Enqueue(event4, gameTime, seconds(1));

			Event<Foo>::Subscribe(countSubscriber);
			Event<EventEnqueueFoo>::Subscribe(clearSubscriber);
			Event<Foo>::ProcessSubscriberCommands();
			Event<EventEnqueueFoo>::ProcessSubscriberCommands();
			gameTime.SetCurrentTime(high_resolution_clock::time_point(1000ms));
			Assert::AreEqual(size_t(0), countSubscriber.Count);
			Assert::AreEqual(size_t(5), eventQueue.Size());
			eventQueue.Update(gameTime);
			Assert::AreEqual(size_t(1), countSubscriber.Count);
			Assert::AreEqual(size_t(0), eventQueue.Size());

			Event<Foo>::UnsubscribeAll();
			Event<EventEnqueueFoo>::Unsubscribe(clearSubscriber);
			Event<EventEnqueueFoo>::UnsubscribeAll();
			Event<Foo>::ProcessSubscriberCommands();
			Event<EventEnqueueFoo>::ProcessSubscriberCommands();

		}

		TEST_METHOD(EventPendingSubscribers)
		{
			GameTime gameTime;
			FooSubscriber fooSubscriber;
			EventAddFooSubscriber addSubscriber;
			EventUnsubscribeFooSubscriber removeSubscriber;
			addSubscriber.sub = &fooSubscriber;
			removeSubscriber.sub = &fooSubscriber;
			EventUnsubscribeAll clearSubscriber;

			// Testing adding a subscriber
			Foo foo(69);
			EventQueue eventQueue;
			shared_ptr<Event<Foo>> event = make_shared<Event<Foo>>(foo);
			Event<Foo>::Subscribe(addSubscriber);
			Event<Foo>::ProcessSubscriberCommands();
			eventQueue.Enqueue(event, gameTime);
			Assert::AreEqual(size_t(1), Event<Foo>::Size());

			gameTime.SetCurrentTime(high_resolution_clock::time_point(1s));
			eventQueue.Update(gameTime);

			Assert::AreEqual(size_t(1), Event<Foo>::Size());
			Event<Foo>::ProcessSubscriberCommands();
			Assert::AreEqual(size_t(2), Event<Foo>::Size());

			// Testing removing a subscriber
			Event<Foo>::Unsubscribe(addSubscriber);
			Event<Foo>::Subscribe(removeSubscriber);
			Event<Foo>::ProcessSubscriberCommands();

			eventQueue.Enqueue(event, gameTime);
			Assert::AreEqual(size_t(2), Event<Foo>::Size());

			gameTime.SetCurrentTime(high_resolution_clock::time_point(2s));
			eventQueue.Update(gameTime);

			Assert::AreEqual(size_t(2), Event<Foo>::Size());
			Event<Foo>::ProcessSubscriberCommands();
			Assert::AreEqual(size_t(1), Event<Foo>::Size());

			// Testing removing all subscribers
			Event<Foo>::Subscribe(clearSubscriber);
			Event<Foo>::ProcessSubscriberCommands();

			eventQueue.Enqueue(event, gameTime);
			Assert::AreEqual(size_t(2), Event<Foo>::Size());

			gameTime.SetCurrentTime(high_resolution_clock::time_point(3s));
			eventQueue.Update(gameTime);

			Assert::AreEqual(size_t(2), Event<Foo>::Size());
			Event<Foo>::ProcessSubscriberCommands();
			Assert::AreEqual(size_t(0), Event<Foo>::Size());

		}


	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState EventTests::_startMemState;
}