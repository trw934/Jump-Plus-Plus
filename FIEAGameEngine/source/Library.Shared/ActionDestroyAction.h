#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionDestroyAction : public FieaGameEngine::Action
	{
		friend class GameState;
		RTTI_DECLARATIONS(ActionDestroyAction, FieaGameEngine::Action);
	public:
		/// <summary>
		/// Constructs an empty ActionDestroyAction
		/// </summary>
		ActionDestroyAction();

		/// <summary>
		/// Defaulted special members
		/// </summary>
		virtual ~ActionDestroyAction() = default;
		ActionDestroyAction(const ActionDestroyAction& other) = default;
		ActionDestroyAction& operator=(const ActionDestroyAction& other) = default;
		ActionDestroyAction(ActionDestroyAction&& other) = default;
		ActionDestroyAction& operator=(ActionDestroyAction&& other) = default;

		/// <summary>
		/// Sets the name of the action to be destroyed
		/// </summary>
		/// <param name="value">New name of the action to be destroyed</param>
		void SetDestroyAction(const std::string& value);

		/// <summary>
		/// Gets the name of the action to be destroyed
		/// </summary>
		/// <returns>The name of the action to be destroyed</returns>
		std::string& GetDestroyAction();

		/// <summary>
		/// Performs any necessary actions to update the GameObject and then calls Update on any child GameObjects.
		/// </summary>
		/// <param name="gameTime">GameTime reference used for updating GameObjects</param>
		virtual void Update(const GameTime& gameTime) override;

		/// <summary>
		/// Generates the list of prescribed attributes and returns it
		/// </summary>
		/// <returns>Vector of prescribed attribute signatures</returns>
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	private:
		inline static Vector<std::pair<FieaGameEngine::Datum*, std::size_t>> destroyedActions;
		std::string _destroyAction = "";
	};
	ConcreteFactory(ActionDestroyAction, FieaGameEngine::Scope);
}