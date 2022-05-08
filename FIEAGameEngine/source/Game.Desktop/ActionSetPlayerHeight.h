#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionSetPlayerHeight : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionSetPlayerHeight, FieaGameEngine::Action);
	public:
		ActionSetPlayerHeight();

		virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		void SetTarget(const std::string& value);
		std::string& Target();

		void SetValue(int value);
		int Value();
	private:
		std::string _target = "PlayerManager";
		int _value = 500;
		FieaGameEngine::Datum* _targetDatum{ nullptr };
		bool _parentFound{ false };
		Scope* _parentScope{ this };
	
	};

	ConcreteFactory(ActionSetPlayerHeight, FieaGameEngine::Scope);
}