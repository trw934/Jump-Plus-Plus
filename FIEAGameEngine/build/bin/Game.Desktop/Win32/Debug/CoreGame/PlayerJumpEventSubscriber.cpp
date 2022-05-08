#include "PlayerJumpEventSubscriber.h"

void PlayerJumpEventSubscriber::Notify(const FieaGameEngine::EventPublisher& event)
{
	//const Event<Player> e = event.As<Event<Player>>();

	//if (e != nullptr)
	//{
	//	//Get player from the message message
	//	const Player& player = e->Message();

	//	//Call the jump method on player/do manual work here
	//	player.Jump();
	// OR
	//	player.y += velocity
	//}

	wasNotified = true;
}
