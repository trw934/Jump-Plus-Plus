#include "pch.h"
#include "AttributedBar.h"

using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(AttributedBar);

	AttributedBar::AttributedBar() :
		AttributedFoo(AttributedBar::TypeIdClass())
	{
	}

	const Vector<Signature> AttributedBar::Signatures()
	{
		return Vector<Signature>
		{
			{ "BarExternalInteger", DatumTypes::Integer, 1, offsetof(AttributedBar, BarExternalInteger) },
		};
	}

	AttributedBar* AttributedBar::Clone() const
	{
		return new AttributedBar(*this);
	}

	bool AttributedBar::Equals(const RTTI* rhs) const
	{
		const auto rhsFoo = rhs->As<AttributedBar>();
		if (rhsFoo == nullptr)
		{
			return false;
		}

		return BarExternalInteger == rhsFoo->BarExternalInteger;
	}

	string AttributedBar::ToString() const
	{
		return "AttributedBar"s;
	}
}