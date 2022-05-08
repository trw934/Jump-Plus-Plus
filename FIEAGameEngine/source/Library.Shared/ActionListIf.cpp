#include "pch.h"
#include "ActionListIf.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListIf);

	ActionListIf::ActionListIf() :
		ActionList(ActionListIf::TypeIdClass())
	{
	}

	int ActionListIf::Condition()
	{
		return _condition;
	}

	void ActionListIf::SetCondition(int result)
	{
		_condition = result;
	}

	const Vector<Signature> ActionListIf::Signatures()
	{
		return Vector<Signature>
		{
			{ "Condition", DatumTypes::Integer, 1, offsetof(ActionListIf, _condition) }
		};
	}

	void ActionListIf::Update(const GameTime& gameTime)
	{
		Datum& actionDatum = Actions();
		int result = (_condition) ? 0 : 1;
		assert(actionDatum.GetTable(result)->Is(Action::TypeIdClass()));
		static_cast<Action*>(actionDatum.GetTable(result))->Update(gameTime);
	}
}