#pragma once
#include "AttributedFoo.h"

namespace UnitTestLibraryDesktop
{
	class AttributedBar final : public AttributedFoo
	{
		RTTI_DECLARATIONS(AttributedBar, AttributedFoo);

	public:
		AttributedBar();
		AttributedBar(const AttributedBar&) = default;
		AttributedBar(AttributedBar&&) = default;
		AttributedBar& operator=(const AttributedBar&) = default;
		AttributedBar& operator=(AttributedBar&&) = default;
		~AttributedBar() = default;

		int BarExternalInteger = 0;

		AttributedBar* Clone() const override;
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;

		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};
}