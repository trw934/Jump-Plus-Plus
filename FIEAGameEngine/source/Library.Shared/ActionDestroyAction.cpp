#include "pch.h"
#include "ActionDestroyAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction);

	ActionDestroyAction::ActionDestroyAction() :
		Action(ActionDestroyAction::TypeIdClass())
	{
	}

	void ActionDestroyAction::SetDestroyAction(const std::string& value)
	{
		_destroyAction = value;
	}

	std::string& ActionDestroyAction::GetDestroyAction()
	{
		return _destroyAction;
	}

	const Vector<Signature> ActionDestroyAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "Destroy Action", DatumTypes::String, 1, offsetof(ActionDestroyAction, _destroyAction) }
		};
	}

	void ActionDestroyAction::Update(const FieaGameEngine::GameTime&)
	{
		Datum* actionsDatum = Search("Actions");
		if (actionsDatum != nullptr)
		{
			for (size_t i = 0; i < actionsDatum->Size(); ++i)
			{
				if (actionsDatum->GetTable(i)->As<Action>()->Name() == _destroyAction)
				{
					destroyedActions.PushBack({ actionsDatum,i });
					break;
				}
			}
		}
	}
}