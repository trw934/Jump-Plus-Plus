#pragma once
#include "GameObject.h"
#include "Level.h"
class LevelManager : public FieaGameEngine::GameObject
{
	RTTI_DECLARATIONS(LevelManager, GameObject);
public: 
	
	LevelManager();
	virtual ~LevelManager() = default;
	LevelManager(const LevelManager& other) = default;
	LevelManager& operator=(const LevelManager& other) = default;
	LevelManager(LevelManager&& other) = default;
	LevelManager& operator=(LevelManager&& other) = default;

	gsl::owner<LevelManager*> Clone() const override;

	static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

	void SwitchActiveLevel();

	GameObject* GetRandomLevel();
	GameObject* GetLevel(size_t levelIndex);
protected:
	FieaGameEngine::Datum* _activeDatum = nullptr;
	FieaGameEngine::Datum* _levelsDatum = nullptr;
};
ConcreteFactory(LevelManager, FieaGameEngine::Scope);
