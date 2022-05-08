#pragma once
#include "GameObject.h"
#include "Factory.h"
class Level : public FieaGameEngine::GameObject
{
	RTTI_DECLARATIONS(Level, GameObject);
public: 
	Level();
	virtual ~Level() = default;
	Level(const Level & other) = default;
	Level& operator=(const Level & other) = default;
	Level(Level && other) = default;
	Level& operator=(Level && other) = default;

	gsl::owner<Level*> Clone() const override;
	virtual void Update(const FieaGameEngine::GameTime& gameTime) override;
	static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
};
ConcreteFactory(Level, FieaGameEngine::Scope);

