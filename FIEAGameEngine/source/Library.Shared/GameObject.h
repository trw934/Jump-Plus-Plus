#pragma once
#include "Attributed.h"
#include "Action.h"
#include "GameClock.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	class GameObject : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(GameObject, FieaGameEngine::Attributed);
	public:
		/// <summary>
		/// Constructs an empty GameObject
		/// </summary>
		GameObject();

		/// <summary>
		/// Defaulted special members
		/// </summary>
		virtual ~GameObject() = default;
		GameObject(const GameObject& other) = default;
		GameObject& operator=(const GameObject& other) = default;
		GameObject(GameObject&& other) = default;
		GameObject& operator=(GameObject&& other) = default;

		/// <summary>
		/// Returns the datum that stores the actions associated with the GameObject
		/// </summary>
		/// <returns>Reference to the datum where actions are stored</returns>
		Datum& Actions();

		/// <summary>
		/// Creates a new action and adopts it into the actions scope of the GameObject
		/// </summary>
		/// <param name="className">Class name of the action</param>
		/// <param name="instanceName">Instance name of the action</param>
		/// <returns>Pointer to the created action</returns>
		Scope* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Creates a new GameObject that is a copy of the current GameObject
		/// </summary>
		/// <returns>Pointer to the created GameObject</returns>
		virtual gsl::owner<GameObject*> Clone() const override;

		/// <summary>
		/// Compares this GameObject to another GameObject, returning true if the addresses are the same
		/// </summary>
		/// <param name="rhs">Other GameObject to compare against</param>
		/// <returns>True if the GameObjects point to the same memory location, false if otherwise</returns>
		virtual bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Returns the name of the object
		/// </summary>
		/// <returns>Name of the object</returns>
		virtual std::string ToString() const override;

		/// <summary>
		/// Performs any necessary actions to update the GameObject and then calls Update on any child GameObjects.
		/// </summary>
		/// <param name="gameTime">GameTime reference used for updating GameObjects</param>
		virtual void Update(const GameTime& gameTime);

		/// <summary>
		/// Returns the position of the object
		/// </summary>
		/// <returns>Vec4 representing the position of the object</returns>
		glm::vec4& GetPosition();

		/// <summary>
		/// Returns the rotation of the object
		/// </summary>
		/// <returns>Vec4 representing the rotation of the object</returns>
		glm::vec4& GetRotation();

		/// <summary>
		/// Returns the scale of the object
		/// </summary>
		/// <returns>Vec4 representing the scale of the object</returns>
		glm::vec4& GetScale();

		/// <summary>
		/// Generates the list of prescribed attributes and returns it
		/// </summary>
		/// <returns>Vector of prescribed attribute signatures</returns>
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		/// <summary>
		/// Name of the object
		/// </summary>
		std::string Name;

		/// <summary>
		/// Transform of the object
		/// </summary>
		glm::mat4 Transform;

		/// <summary>
		/// Tag associated with the object
		/// </summary>
		std::string Tag;

		/// <summary>
		/// File destination of the sprite of the game object
		/// </summary>
		std::string Sprite;

		int SpriteWidth;
		int SpriteHeight;

	protected:
		std::chrono::high_resolution_clock::time_point prevTimePoint;
		GameObject(size_t childID);
	};

	ConcreteFactory(GameObject, FieaGameEngine::Scope)
}