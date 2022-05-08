#include "pch.h"
#include "Avatar.h"

using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(Avatar);

	Avatar::Avatar() :
		GameObject(Avatar::TypeIdClass())
	{
	}

	Vector<Signature> Avatar::Signatures()
	{
		return Vector<Signature>
		{
			{ "HitPoints", DatumTypes::Integer, 1, offsetof(Avatar, HitPoints) },
		};
	}

	gsl::owner<Avatar*> Avatar::Clone() const
	{
		return new Avatar(*this);
	}

	void Avatar::Update(const FieaGameEngine::GameTime& gameTime)
	{
		--HitPoints;
		Datum* childrenDatum = Find("Children");
		if (childrenDatum != nullptr)
		{
			for (size_t i = 1; i < childrenDatum->Size(); ++i)
			{
				childrenDatum->GetTable(i)->As<GameObject>()->Update(gameTime);
			}
		}
	}
}