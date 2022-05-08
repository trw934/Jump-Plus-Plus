#include "pch.h"
#include "ActionIncrement.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement);

	ActionIncrement::ActionIncrement() :
		Action(ActionIncrement::TypeIdClass())
	{
	}

	Vector<Signature> ActionIncrement::Signatures()
	{
		return Vector<Signature>
		{
			{ "Target", DatumTypes::String, 1, offsetof(ActionIncrement, _target) },
			{ "Step", DatumTypes::Float, 1, offsetof(ActionIncrement, _step)}
		};
	}

	void ActionIncrement::SetTarget(const std::string& value)
	{
		_target = value;
		_targetDatum = nullptr;
	}

	std::string& ActionIncrement::Target()
	{
		return _target;
	}

	void ActionIncrement::SetStep(float step)
	{
		_step = step;
	}

	float ActionIncrement::Step()
	{
		return _step;
	}

	void ActionIncrement::Update(const FieaGameEngine::GameTime&)
	{
		if (_targetDatum == nullptr)
		{
			_targetDatum = Search(_target);
		}
		if (_targetDatum != nullptr)
		{
			if (_targetDatum->Type() == DatumTypes::Integer)
			{
				_targetDatum->GetInt() += (int)_step;
			}
			else
			{
				_targetDatum->GetFloat() += _step;
			}
		}
		else
		{
			throw std::runtime_error("Target does not exist.");
		}
	}
}