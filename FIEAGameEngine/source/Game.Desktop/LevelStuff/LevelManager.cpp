#include "pch.h"
#include "LevelManager.h"

using namespace FieaGameEngine;

RTTI_DEFINITIONS(LevelManager);

LevelManager::LevelManager() :
	GameObject(LevelManager::TypeIdClass())
{
	/*Level level = *GetRandomLevel()->As<Level>();
	Find("ActiveLevels")->PushBack(&level);*/
}

gsl::owner<LevelManager*> LevelManager::Clone() const
{
	return new LevelManager(*this);
}

Vector<Signature> LevelManager::Signatures()
{
	return Vector<Signature>
	{
		{ "Levels", DatumTypes::Table, 0, 0 },
		{ "ActiveLevels", DatumTypes::Table, 0, 0}
	};
}

void LevelManager::Update(const FieaGameEngine::GameTime& gameTime)
{
	GameObject::Update(gameTime);
	Datum* levels = Find("ActiveLevels");
	if (levels != nullptr)
	{
		levels->GetTable(0)->As<GameObject>()->Update(gameTime);
	}
}

void LevelManager::SwitchActiveLevel()
{
	if (_activeDatum == nullptr)
	{
		_activeDatum = Find("ActiveLevels");
	}
	if (_activeDatum != nullptr)
	{
		_activeDatum->RemoveAt(0);
		Adopt(*GetRandomLevel()->Clone(), "ActiveLevels");
	}
}

GameObject* LevelManager::GetRandomLevel()
{
	if (_levelsDatum == nullptr)
	{
		_levelsDatum = Find("Levels");
	}
	assert(_levelsDatum != nullptr);
	return GetLevel(rand() % _levelsDatum->Size());
}

GameObject* LevelManager::GetLevel(size_t levelIndex)
{
	if (_levelsDatum == nullptr)
	{
		_levelsDatum = Find("Levels");
	}
	assert(_levelsDatum != nullptr);
	return _levelsDatum->GetTable(levelIndex)->As<GameObject>();
}
