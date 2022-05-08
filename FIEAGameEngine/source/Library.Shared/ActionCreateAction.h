#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionCreateAction : public FieaGameEngine::Action
	{
		friend class GameState;
		RTTI_DECLARATIONS(ActionCreateAction, FieaGameEngine::Action);
	public:
		/// <summary>
		/// Constructs an empty ActionCreateAction
		/// </summary>
		ActionCreateAction();

		/// <summary>
		/// Defaulted special members
		/// </summary>
		virtual ~ActionCreateAction() = default;
		ActionCreateAction(const ActionCreateAction& other) = default;
		ActionCreateAction& operator=(const ActionCreateAction& other) = default;
		ActionCreateAction(ActionCreateAction&& other) = default;
		ActionCreateAction& operator=(ActionCreateAction&& other) = default;

		/// <summary>
		/// Sets the created action's name
		/// </summary>
		/// <param name="value">The name to set the created action's name to</param>
		void SetActionName(const std::string& value);

		/// <summary>
		/// Gets the created action's name
		/// </summary>
		/// <returns>The name of the created action</returns>
		std::string& ActionName();

		/// <summary>
		/// Sets the created action prototype value
		/// </summary>
		/// <param name="value">The new action prototype value</param>
		void SetPrototype(const std::string& value);

		/// <summary>
		/// Gets the created action prototype value
		/// </summary>
		/// <returns>The created action prototype value</returns>
		std::string& Prototype();

		/// <summary>
		/// Creates the action and pushes it and a pointer to where it will be instantiated to the vector of created actions
		/// </summary>
		/// <param name="gameTime">GameTime reference used for updating GameObjects</param>
		virtual void Update(const GameTime& gameTime) override;

		/// <summary>
		/// Generates the list of prescribed attributes and returns it
		/// </summary>
		/// <returns>Vector of prescribed attribute signatures</returns>
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	private:
		inline static Vector<std::pair<Scope*, Action*>> createdActions;
		std::string _actionName = "";
		std::string _prototype = "";
	};
	ConcreteFactory(ActionCreateAction, FieaGameEngine::Scope);
}