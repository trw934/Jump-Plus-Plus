#pragma once
#include "GameObject.h"
#include "..\\Rendering\Renderer.h"

class Platform : public FieaGameEngine::GameObject
{
	RTTI_DECLARATIONS(Platform, GameObject);
public:
	Platform();
	virtual ~Platform() = default;
	Platform(const Platform& other) = default;
	Platform& operator=(const Platform& other) = default;
	Platform(Platform&& other) = default;
	Platform& operator=(Platform&& other) = default;

	gsl::owner<Platform*> Clone() const override;

	virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

	static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	int GetSpriteWidth() const;
	int GetSpriteHeight() const;
protected:
	Platform(std::size_t childID);
	int _spriteWidth = 52;
	int _spriteHeight = 13;
};

ConcreteFactory(Platform, FieaGameEngine::Scope);
