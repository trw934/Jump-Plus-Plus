#include "pch.h"
#include "Platform.h"

using namespace FieaGameEngine;

RTTI_DEFINITIONS(Platform);

Platform::Platform() :
	GameObject(Platform::TypeIdClass())
{
}

Platform::Platform(size_t childID) :
	GameObject(childID)
{
}

gsl::owner<Platform*> Platform::Clone() const
{
	return new Platform(*this);
}

void Platform::Update(const GameTime&)
{
	glm::vec4& position = GetPosition();
	Renderer::GetInstance()->Draw(Sprite, static_cast<int>(position.x), static_cast<int>(position.y), _spriteWidth, _spriteHeight);
}

Vector<Signature> Platform::Signatures()
{
	return Vector<Signature>
	{
	};
}

int Platform::GetSpriteWidth() const
{
	return _spriteWidth;
}

int Platform::GetSpriteHeight() const
{
	return _spriteHeight;
}

