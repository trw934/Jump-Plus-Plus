#pragma once
#include "GameObject.h"

namespace UnitTestLibraryDesktop
{
	class Avatar final : public FieaGameEngine::GameObject
	{
		RTTI_DECLARATIONS(Avatar, FieaGameEngine::GameObject);

	public:
		Avatar();

		gsl::owner<Avatar*> Clone() const override;

		int HitPoints{ 50 };

		virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};

	ConcreteFactory(Avatar, FieaGameEngine::Scope);
}