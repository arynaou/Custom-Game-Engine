#pragma once

#include <cstdint>
#include "RTTI.h"
#include "Factory.h"

namespace UnitTests
{
	class Foo : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Foo, FieaGameEngine::RTTI)

	public:
		Foo(std::int32_t data = 0);

		/// <summary>
		/// Instantiate a Foo from a existing instance
		/// </summary>
		/// <param name="other"></param>
		Foo(const Foo& other);

		/// <summary>
		/// Assign a Foo
		/// </summary>
		/// <returns>The current instance (i.e. the left-hand side of the assignment)</returns>
		Foo& operator=(const Foo& other);

		~Foo();

		/// <summary>
		/// 
		/// </summary>
		/// <returns>return the data in Foo</returns>
		int& Data();

		/// <summary>
		/// 
		/// </summary>
		/// <returns>return the data in Foo</returns>
		int& Data() const;

		/// <summary>
		/// compare if two Foo are equal
		/// </summary>
		/// <param name="rhs">the foo you compare with</param>
		/// <returns>if the two foo are equal</returns>
		bool operator==(const Foo& rhs) const;

		/// <summary>
		/// compare if two Foo are not equal
		/// </summary>
		/// <param name="rhs">the foo you compare with</param>
		/// <returns>if the two foo are not equal</returns>
		bool operator!=(const Foo& rhs) const;

	private:	
		std::int32_t* _data;
		
	};

	ConcreteFactory(Foo, FieaGameEngine::RTTI)
}