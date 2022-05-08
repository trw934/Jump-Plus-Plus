#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionIncrement : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionIncrement, FieaGameEngine::Action);
	public:
		ActionIncrement();

		virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		void SetTarget(const std::string& value);
		std::string& Target();

		void SetStep(float step);
		float Step();
	private:
		std::string _target = "None";
		float _step = 1.0;
		FieaGameEngine::Datum* _targetDatum{ nullptr };
	};

	ConcreteFactory(ActionIncrement, FieaGameEngine::Scope);
}