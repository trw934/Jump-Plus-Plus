#pragma once
#include "Attributed.h"
#include "GameClock.h"

namespace FieaGameEngine
{
	class Action : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(Action, FieaGameEngine::Attributed);
	public:
		/// <summary>
		/// Defaulted special members
		/// </summary>
		virtual ~Action() = default;
		Action(const Action& other) = default;
		Action& operator=(const Action& other) = default;
		Action(Action&& other) = default;
		Action& operator=(Action&& other) = default;

		/// <summary>
		/// Performs any necessary actions to update the action and then calls Update on any child actions.
		/// </summary>
		/// <param name="gameTime">GameTime reference used for updating actions</param>
		virtual void Update(const GameTime& gameTime) = 0;

		/// <summary>
		/// Generates the list of prescribed attributes and returns it
		/// </summary>
		/// <returns>Vector of prescribed attribute signatures</returns>
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		/// <summary>
		/// Sets the name of the action
		/// </summary>
		/// <param name="value">Reference to the new name of the action</param>
		void SetName(const std::string& value);

		/// <summary>
		/// Returns a reference to the action's name
		/// </summary>
		/// <returns>The name of the action</returns>
		std::string& Name();

	protected:
		explicit Action(size_t childID);
	private:
		std::string _name = "";
	};
}