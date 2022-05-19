#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "GameTime.h"
#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine
{
	struct Transform final
	{
		glm::vec4 Position{0,0,0,0};
		glm::vec4 Rotation{ 0,0,0,0 };
		glm::vec4 Scale{ 0,0,0,0 };
	};
	
	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed);

	public:
		/// <summary>
		/// Game Object Constructor
		/// </summary>
		GameObject();
		virtual ~GameObject() override = default;

		/// <summary>
		/// Game Object copy constructor
		/// </summary>
		/// <param name="rhs">the game object intend to copy</param>
		GameObject(const GameObject& rhs) = default;

		/// <summary>
		/// Game Object move constructor
		/// </summary>
		/// <param name="rhs">Game Object intend to move</param>
		GameObject(GameObject&& rhs) noexcept = default;

		/// <summary>
		/// Game Object copy assignment
		/// </summary>
		/// <param name="rhs">the game object intend to copy</param>
		/// <returns>after copying the game object reference</returns>
		GameObject & operator=(const GameObject & rhs) = default;

		/// <summary>
		/// Game Object move assignment
		/// </summary>
		/// <param name="rhs">the game object intend to copy</param>
		/// <returns>after moving the game object reference</returns>
		GameObject& operator=(GameObject&& rhs) noexcept = default;

		/// <summary>
		/// Game Object transform, including three vector4: Position, Rotation and Scale
		/// </summary>
		Transform Transform;

		/// <summary>
		/// Game Object's name
		/// </summary>
		std::string Name {"GameObject"s};

		/// <summary>
		/// Clone the Game Object
		/// </summary>
		/// <returns>the pointer of game object</returns>
		gsl::owner<GameObject*> Clone() const override;

		/// <summary>
		/// Compare the Game Object to the RTTI pointer
		/// </summary>
		/// <param name="rhs">an RTTI pointer</param>
		/// <returns>if it's equal or not</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Get the game object name
		/// </summary>
		/// <returns>the name of game object</returns>
		std::string ToString() const override;

		/// <summary>
		/// Get the Signature of the game object
		/// </summary>
		/// <returns>a vector of signature</returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Update function that will invoke all the children object's Update
		/// </summary>
		/// <param name="gametime">the gametime</param>
		virtual void Update(const GameTime& gametime);

		Datum& GetActions();

		void CreateAction(const std::string& className, const std::string& name);

	protected:
		/// <summary>
		/// Game Object constructor with parameter typeID
		/// </summary>
		/// <param name="typeID">a size_t typeID</param>
		GameObject(std::size_t typeID);

		Datum& Actions{ At("Actions"s) };

	private:
		Datum& Children{ At("Children"s)};
	};

	ConcreteFactory(GameObject, Scope);
}