#pragma once
#include "ActionList.h"

namespace FieaGameEngine
{
	class ActionListIf final : public FieaGameEngine::ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, FieaGameEngine::ActionList);
	public:
		/// <summary>
		/// Constructs an empty ActionListIF
		/// </summary>
		ActionListIf();

		/// <summary>
		/// Defaulted special members
		/// </summary>
		virtual ~ActionListIf() = default;
		ActionListIf(const ActionListIf& other) = default;
		ActionListIf& operator=(const ActionListIf& other) = default;
		ActionListIf(ActionListIf&& other) = default;
		ActionListIf& operator=(ActionListIf&& other) = default;

		/// <summary>
		/// Gets the value of the condition member
		/// </summary>
		/// <returns>Value of the condition member</returns>
		int Condition();

		/// <summary>
		/// Sets the value of the condition member
		/// </summary>
		/// <param name="result">Value to set the member to</param>
		void SetCondition(int result);

		/// <summary>
		/// Calls update on the action in position 0 of the actions datum if condition is true, and position 1 if the action is false
		/// </summary>
		/// <param name="gameTime">GameTime reference to pass to update calls</param>
		virtual void Update(const GameTime& gameTime) override;

		/// <summary>
		/// Generates the list of prescribed attributes and returns it
		/// </summary>
		/// <returns>Vector of prescribed attribute signatures</returns>
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	private:
		int _condition{ 1};
	};

	ConcreteFactory(ActionListIf, FieaGameEngine::Scope)
}