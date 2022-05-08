#pragma once
#include "Action.h"
#include "GameClock.h"

namespace FieaGameEngine
{
	class ActionList : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionList, FieaGameEngine::Action);
	public:
		/// <summary>
		/// Constructs an empty ActionList
		/// </summary>
		ActionList();

		/// <summary>
		/// Defaulted special members
		/// </summary>
		virtual ~ActionList() = default;
		ActionList(const ActionList& other) = default;
		ActionList& operator=(const ActionList& other) = default;
		ActionList(ActionList&& other) = default;
		ActionList& operator=(ActionList&& other) = default;

		/// <summary>
		/// Returns the datum that stores the actions associated with the list
		/// </summary>
		/// <returns>Reference to the datum where actions are stored</returns>
		Datum& Actions();

		/// <summary>
		/// Creates a new action and adopts it into the actions scope of the ActionList
		/// </summary>
		/// <param name="className">Class name of the action</param>
		/// <param name="instanceName">Instance name of the action</param>
		/// <returns>Pointer to the created action</returns>
		Scope* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Calls Update on any actions in the ActionList.
		/// </summary>
		/// <param name="gameTime">GameTime reference used for updating Actions</param>
		virtual void Update(const GameTime& gameTime);

		/// <summary>
		/// Creates a new ActionList that is a copy of the current ActionList
		/// </summary>
		/// <returns>Pointer to the created ActionList</returns>
		virtual gsl::owner<ActionList*> Clone() const;

		/// <summary>
		/// Generates the list of prescribed attributes and returns it
		/// </summary>
		/// <returns>Vector of prescribed attribute signatures</returns>
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	protected:
		ActionList(size_t childID);
	};

	ConcreteFactory(ActionList, FieaGameEngine::Scope)
}