#include "pch.h"
#include "ActionSetPlayerHeight.h"
#include "Player/PlayerManager.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionSetPlayerHeight);

	ActionSetPlayerHeight::ActionSetPlayerHeight() :
		Action(ActionSetPlayerHeight::TypeIdClass())
	{
	}

	Vector<Signature> ActionSetPlayerHeight::Signatures()
	{
		return Vector<Signature>
		{
			{ "Target", DatumTypes::String, 1, offsetof(ActionSetPlayerHeight, _target) },
			{ "Value", DatumTypes::Integer, 1, offsetof(ActionSetPlayerHeight, _value) }
		};
	}

	void ActionSetPlayerHeight::SetTarget(const std::string& value)
	{
		_target = value;
		_targetDatum = nullptr;
	}

	std::string& ActionSetPlayerHeight::Target()
	{
		return _target;
	}

	void ActionSetPlayerHeight::SetValue(int step)
	{
		_value = step;
	}

	int ActionSetPlayerHeight::Value()
	{
		return _value;
	}

	void ActionSetPlayerHeight::Update(const FieaGameEngine::GameTime&)
	{
		if (_targetDatum == nullptr) 
		{
			if (!_parentFound)
			{
				while (_parentScope != nullptr)
				{
					_targetDatum = _parentScope->Search(_target);

					for (std::size_t i = 0; i < _targetDatum->Size(); ++i)
					{
						if (_targetDatum->GetTable()->Find("Name")->GetString() == "Player")
						{
							_parentFound = true;
							PlayerManager& player = *_targetDatum->GetTable(0)->As<PlayerManager>();
							player.lastHeight = _value;

						}
					}
					if (_parentFound) { break; }
					else
					{
						_parentScope = _parentScope->GetParent();
					}
				}
			}
		}
		

		else
		{
			PlayerManager& player = *_targetDatum->GetTable(0)->As<PlayerManager>();
			player.lastHeight = _value;
		}
	}
}