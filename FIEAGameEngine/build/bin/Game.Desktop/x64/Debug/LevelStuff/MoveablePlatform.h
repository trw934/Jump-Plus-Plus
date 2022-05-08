#pragma once
#include "GameObject.h"
#include "Platform.h"

class MoveablePlatform : public Platform
{
	RTTI_DECLARATIONS(MoveablePlatform, Platform);

public:
	MoveablePlatform();
	virtual ~MoveablePlatform() = default;
	MoveablePlatform(const MoveablePlatform& other) = default;
	MoveablePlatform& operator=(const MoveablePlatform& other) = default;
	MoveablePlatform(MoveablePlatform&& other) = default;
	MoveablePlatform& operator=(MoveablePlatform&& other) = default;

	gsl::owner<MoveablePlatform*> Clone() const override;

	virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

	static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

protected:
	float MoveSpeed{ 1.f };	//user defined movement speed of the platform
	glm::vec4 MovePosition{ 0 }; //the location platform will move to

private:
	glm::vec4 _startPosition;  //will save the first position
	bool _isReached{false};  //if the platform is moving to the MovePosition
	float _movementVariable{ 0.01f };  // the variable can change the speed of the actual movement(should be less than 1)

	bool _cachedStartPos{false};

	void Move(glm::vec4 start, glm::vec4 end);  //simplify the code, it will control the platform movement
};

ConcreteFactory(MoveablePlatform, FieaGameEngine::Scope);