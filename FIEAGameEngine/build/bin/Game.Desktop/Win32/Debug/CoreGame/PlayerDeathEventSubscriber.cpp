#include "PlayerDeathEventSubscriber.h"

void PlayerDeathEventSubscriber::Notify(const FieaGameEngine::EventPublisher& event)
{
	//const Event<Player> e = event.As<Event<Player>>();

	//if (e != nullptr)
	//{
	//	//Get player from the message message
	//	const Player& player = e->Message();

	//	//Call the Die/Reset methods for player from here
	//	player.Die();
	// OR
	//	player.Reset();
	//}

	wasNotified = true;
}
