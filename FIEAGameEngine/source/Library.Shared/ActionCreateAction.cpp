#include "pch.h"
#include "ActionCreateAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction);

	ActionCreateAction::ActionCreateAction() :
		Action(ActionCreateAction::TypeIdClass())
	{
	}

	void ActionCreateAction::SetActionName(const std::string& value)
	{
		_actionName = value;
	}

	std::string& ActionCreateAction::ActionName()
	{
		return _actionName;
	}

	void ActionCreateAction::SetPrototype(const std::string& value)
	{
		_prototype = value;
	}

	std::string& ActionCreateAction::Prototype()
	{
		return _prototype;
	}

	const Vector<Signature> ActionCreateAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "Action Name", DatumTypes::String, 1, offsetof(ActionCreateAction, _actionName) },
			{ "Prototype", DatumTypes::String, 1, offsetof(ActionCreateAction, _prototype) }
		};
	}

	void ActionCreateAction::Update(const FieaGameEngine::GameTime&)
	{
		Action* newAction = Factory<Scope>::Create(_prototype)->As<Action>();
		assert(newAction->Is("Action"));
		newAction->SetName(_actionName);
		createdActions.PushBack({ _parent, newAction });
	}
}