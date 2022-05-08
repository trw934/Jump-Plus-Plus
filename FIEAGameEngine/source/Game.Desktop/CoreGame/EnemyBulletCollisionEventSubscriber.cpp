#include "EnemyBulletCollisionEventSubscriber.h"

void EnemyBulletCollisionEventSubscriber::Notify(const FieaGameEngine::EventPublisher& event)
{
	//const Event<Enemy> e = event.As<Event<Enemy>>();

	//if (e != nullptr)
	//{
	//	//Get player from the message message
	//	const Enemy& enemy= e->Message();

	//	//Call Destroy method on enemy/do work here
	//	enemy.Destroy();
	//}

	wasNotified = true;
}
