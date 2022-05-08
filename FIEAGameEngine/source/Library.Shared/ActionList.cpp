#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList);

	ActionList::ActionList() :
		Action(ActionList::TypeIdClass())
	{
	}

	ActionList::ActionList(size_t childID) :
		Action(childID)
	{
	}

	const Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			{ "Actions", DatumTypes::Table, 0, 0 }
		};
	}

	gsl::owner<ActionList*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	Datum& ActionList::Actions()
	{
		Datum* result = Find("Actions");
		assert(result != nullptr);
		return *result;
	}

	Scope* ActionList::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Scope* newAction = Factory<Scope>::Create(className);
		assert(newAction->Is("Action"));
		Adopt(*newAction, "Actions");
		newAction->As<Action>()->SetName(instanceName);
		return newAction;
	}

	void ActionList::Update(const GameTime& gameTime)
	{
		Datum& actionDatum = Actions();
		for (size_t i = 0; i < actionDatum.Size(); ++i)
		{
			assert(actionDatum.GetTable(i)->Is(Action::TypeIdClass()));
			static_cast<Action*>(actionDatum.GetTable(i))->Update(gameTime);
		}
	}
}