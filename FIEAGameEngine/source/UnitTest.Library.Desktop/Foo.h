#pragma once
#include <cstdint>
#include "RTTI.h"
#include "Factory.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	class Foo : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI);
	public:
		Foo() = default;
		Foo(std::int32_t data);
		Foo(const Foo& other);
		Foo& operator=(const Foo& other);
		Foo(Foo&& other) noexcept;
		Foo& operator=(Foo&& other) noexcept;
		~Foo();

		int& Data();
		const int& Data() const;

		bool operator==(const Foo& rhs) const;
		bool operator!=(const Foo& rhs) const;

	private:
		std::int32_t* mData{ nullptr };
	};

	ConcreteFactory(Foo, FieaGameEngine::RTTI)
}