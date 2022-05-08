#include "pch.h"
#include "Level.h"

using namespace FieaGameEngine;

RTTI_DEFINITIONS(Level);

Level::Level() : 
	GameObject(Level::TypeIdClass())
{
}

gsl::owner<Level*> Level::Clone() const
{
	return new Level(*this);
}

void Level::Update(const FieaGameEngine::GameTime& gameTime)
{
	GameObject::Update(gameTime);
	Datum* platforms = Find("Platforms");
	if (platforms != nullptr)
	{
		for (size_t i = 0; i < platforms->Size(); ++i)
		{
			platforms->GetTable(i)->As<GameObject>()->Update(gameTime);
		}
	}
	Datum* enemies = Find("Enemies");
	if (enemies != nullptr)
	{
		for (size_t i = 0; i < enemies->Size(); ++i)
		{
			enemies->GetTable(i)->As<GameObject>()->Update(gameTime);
		}
	}
	Datum* player = Find("Player");
	if (player != nullptr)
	{
		player->GetTable(0)->As<GameObject>()->Update(gameTime);
	}
	Datum* bullet = Find("Bullet");
	if (bullet != nullptr)
	{
		bullet->GetTable(0)->As<GameObject>()->Update(gameTime);
	}
}

Vector<Signature> Level::Signatures()
{
	return Vector<Signature>
	{
		{ "Platforms", DatumTypes::Table, 0, 0 },
		{ "Enemies", DatumTypes::Table, 0, 0 },
	};
}
