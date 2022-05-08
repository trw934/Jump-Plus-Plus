#pragma once
#include "Attributed.h"
#include "Factory.h"

namespace UnitTestLibraryDesktop
{
	class AttributedFoo : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed);

	public:
		static const std::size_t ArraySize = 5;

		AttributedFoo();
		AttributedFoo(const AttributedFoo&) = default;
		AttributedFoo(AttributedFoo&&) = default;
		AttributedFoo& operator=(const AttributedFoo&) = default;
		AttributedFoo& operator=(AttributedFoo&&) = default;
		virtual ~AttributedFoo() = default;

		int ExternalInteger = 0;
		float ExternalFloat = 0;
		std::string ExternalString;
		glm::vec4 ExternalVector;
		glm::mat4 ExternalMatrix;

		int ExternalIntegerArray[ArraySize];
		float ExternalFloatArray[ArraySize];
		std::string ExternalStringArray[ArraySize];
		glm::vec4 ExternalVectorArray[ArraySize];
		glm::mat4 ExternalMatrixArray[ArraySize];

		gsl::owner<AttributedFoo*> Clone() const override;
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;

		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	protected:
		AttributedFoo(std::size_t childID);
	};

	ConcreteFactory(AttributedFoo, FieaGameEngine::Scope)
}