#include "Scope.h"

namespace FieaGameEngine
{
	template <typename T>
	inline T& Scope::GetParent() const
	{
		assert(_parent != nullptr);
		assert(_parent->Is(T::TypeIdClass()));
		return static_cast<T&>(*_parent);
	}
}