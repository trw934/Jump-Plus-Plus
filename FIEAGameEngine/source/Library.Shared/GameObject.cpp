#include "pch.h"
#include "GameObject.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(GameObject);

	GameObject::GameObject() :
		Attributed(GameObject::TypeIdClass())
	{
	}

	GameObject::GameObject(size_t childID) :
		Attributed(childID)
	{
	}

	gsl::owner<GameObject*> GameObject::Clone() const
	{
		return new GameObject(*this);
	}

	const Vector<Signature> GameObject::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", DatumTypes::String, 1, offsetof(GameObject, Name) },
			{ "Tag", DatumTypes::String, 1, offsetof(GameObject, Tag) },
			{ "Transform", DatumTypes::Matrix, 1, offsetof(GameObject, Transform) },
			{ "Sprite", DatumTypes::String, 1, offsetof(GameObject, Sprite)},
			{ "Children", DatumTypes::Table, 0, 0},
			{ "Actions", DatumTypes::Table, 0, 0},
			{ "Width",DatumTypes::Integer, 1, offsetof(GameObject,SpriteWidth)},
			{ "Height",DatumTypes::Integer, 1, offsetof(GameObject,SpriteHeight)}
		};
	}

	Datum& GameObject::Actions()
	{
		Datum* result = Find("Actions");
		assert(result != nullptr);
		return *result;
	}

	Scope* GameObject::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Scope* newAction = Factory<Scope>::Create(className);
		assert(newAction->Is("Action"));
		Adopt(*newAction, "Actions");
		newAction->As<Action>()->SetName(instanceName);
		return newAction;
	}

	glm::vec4& GameObject::GetPosition()
	{
		return Transform[0];
	}

	glm::vec4& GameObject::GetRotation()
	{
		return Transform[1];
	}

	glm::vec4& GameObject::GetScale()
	{
		return Transform[2];
	}

	void GameObject::Update(const GameTime& gameTime)
	{
		Datum& actionDatum = Actions();
		for (size_t i = 0; i < actionDatum.Size(); ++i)
		{
			assert(actionDatum.GetTable(i)->Is(Action::TypeIdClass()));
			static_cast<Action*>(actionDatum.GetTable(i))->Update(gameTime);
		}

		Datum* childrenDatum = Find("Children");
		if (childrenDatum != nullptr)
		{
			for (size_t i = 0; i < childrenDatum->Size(); ++i)
			{
				assert(childrenDatum->GetTable(i)->Is(GameObject::TypeIdClass()));
				static_cast<GameObject*>(childrenDatum->GetTable(i))->Update(gameTime);
			}
		}
		prevTimePoint = gameTime.CurrentTime();
	}

	bool GameObject::Equals(const RTTI* rhs) const
	{
		const auto rhsAtt = rhs->As<GameObject>();
		if (rhsAtt == nullptr)
		{
			return false;
		}
		return this == rhsAtt;
	}

	std::string GameObject::ToString() const
	{
		return Name;
	}

}