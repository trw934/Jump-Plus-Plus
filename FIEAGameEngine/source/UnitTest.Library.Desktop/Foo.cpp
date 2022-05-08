#include "pch.h"
#include "Foo.h"

using namespace std;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(Foo);

	Foo::Foo(int32_t data) :
		mData(new int32_t(data))
	{
	}

	Foo::Foo(const Foo& other) :
		mData(new int32_t(*other.mData))
	{
	}

	Foo& Foo::operator=(const Foo& other)
	{
		if (this != &other)
		{
			*mData = *other.mData;
		}

		return *this;
	}

	Foo::Foo(Foo&& other) noexcept :
		mData(other.mData)
	{
		other.mData = nullptr;
	}

	Foo& Foo::operator=(Foo&& other) noexcept
	{
		if (this != &other)
		{
			delete mData;
			mData = other.mData;
			other.mData = nullptr;
		}
		return *this;
	}

	Foo::~Foo()
	{
		delete mData;
	}

	int32_t& Foo::Data()
	{
		return *mData;
	}

	const int32_t& Foo::Data() const
	{
		return *mData;
	}

	bool Foo::operator==(const Foo& rhs) const
	{
		if (mData == rhs.mData)
		{
			return true;
		}

		return (*mData == *rhs.mData);
	}

	bool Foo::operator!=(const Foo& rhs) const
	{
		return !operator==(rhs);
	}
}