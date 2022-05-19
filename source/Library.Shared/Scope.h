#pragma once

#include "Datum.h"
#include "HashMap.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		/// <summary>
		/// Scope Constructor which takes the scope parent and the capacity of the scope
		/// </summary>
		Scope(Scope* parent= nullptr, std::size_t capacity = 11);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">the scope intend to copy</param>
		Scope(const Scope& other);

		/// <summary>
		/// Move Consturctor
		/// </summary>
		/// <param name="other">the scope intend to move</param>
		Scope(Scope&& other) noexcept;

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="other">the scope intend to copy</param>
		/// <returns>scope refernce</returns>
		Scope& operator=(Scope&& other) noexcept;

		virtual ~Scope();
		
		/// <summary>
		/// Move Assignment
		/// </summary>
		/// <param name="other">the scope intend to move</param>
		/// <returns>scope reference</returns>
		Scope& operator=(const Scope& other);

		/// <summary>
		/// Find the datum store in the specific key
		/// </summary>
		/// <param name="key">the key intend to find, should be a string</param>
		/// <returns>the Datum pointer</returns>
		Datum* Find(const std::string& key);

		/// <summary>
		/// Find the datum store in the specific key
		/// </summary>
		/// <param name="key">the key intend to find, should be a string</param>
		/// <returns>the Datum pointer</returns>
		const Datum* Find(const std::string& key) const;

		/// <summary>
		/// Search the datum store in the specific key, which will also search form the parents
		/// </summary>
		/// <param name="key">the key intend to find, should be a string</param>
		/// <returns>the Datum pointer</returns>
		Datum* Search(const std::string& key);

		/// <summary>
		/// Search the datum store in the specific key, which will also search form the parents
		/// </summary>
		/// <param name="key">the key intend to find, should be a string</param>
		/// <returns>the Datum pointer</returns>
		const Datum* Search(const std::string& key) const;

		/// <summary>
		/// Search the datum store in the specific key, which will also search form the parents
		/// </summary>
		/// <param name="key">the key intend to find, should be a string</param>
		/// <param name="foundScope">the address of a Scope double pointer variable</param>
		/// <returns>the Datum Pointer</returns>
		Datum* Search(const std::string& key, Scope*& foundScope);

		/// <summary>
		/// Search the datum store in the specific key, which will also search form the parents
		/// </summary>
		/// <param name="key">the key intend to find, should be a string</param>
		/// <param name="foundScope">the address of a Scope double pointer variable</param>
		/// <returns>the Datum Pointer</returns>
		const Datum* Search(const std::string& key,const Scope*& foundScope) const;

		/// <summary>
		/// append a string and create a datum in the Scope
		/// </summary>
		/// <param name="key">the key of the datum you create, it should be a string</param>
		/// <returns>the datum reference</returns>
		Datum& Append(const std::string& key);

		/// <summary>
		/// append a string and create a scope in the Scope 
		/// </summary>
		/// <param name="key">the key of the datum you create, it should be a string</param>
		/// <returns>the datum reference</returns>
		Scope& AppendScope(const std::string& key);

		/// <summary>
		/// Orphan the scope and Append it to this scope
		/// </summary>
		/// <param name="scope">the scope intend to append</param>
		/// <param name="key">the key you're going to save</param>
		void Adopt(Scope& scope, const std::string& key);

		/// <summary>
		/// Get this scope's parent
		/// </summary>
		/// <returns>the Scope pointer of the parent</returns>
		Scope* GetParent();

		/// <summary>
		/// Get this scope's parent
		/// </summary>
		/// <returns>the Scope pointer of the parent</returns>
		Scope* GetParent() const;

		template<typename T>
		T& GetParent();

		/// <summary>
		/// Set this scope's parent
		/// </summary>
		/// <param name="parent">the parent pointer you intend to set</param>
		void SetParent(Scope* parent);

		/// <summary>
		/// append a string and create a datum in the Scope
		/// </summary>
		/// <param name="key">the key of the datum you create, it should be a string</param>
		/// <returns>the datum reference</returns>
		Datum& operator[](const std::string& string);

		Datum& At(const std::string& string);
		const Datum& At(const std::string& string) const;

		/// <summary>
		///  choose an index and create a datum in the Scope
		/// </summary>
		/// <param name="index">the index you intend to insert</param>
		/// <returns>Datum reference</returns>
		Datum& operator[](std::size_t index);

		/// <summary>
		/// the == operator for scope
		/// </summary>
		/// <param name="rhs">the scope reference you want to compare</param>
		/// <returns>if the scope is equal</returns>
		bool operator==(const Scope& rhs) const;

		/// <summary>
		/// the != operator for scope
		/// </summary>
		/// <param name="rhs">the scope reference you want to compare</param>
		/// <returns>if the scope is not equal</returns>
		bool operator!=(const Scope& rhs) const;

		/// <summary>
		/// Find if the Scope has the scope you intend to find
		/// </summary>
		/// <param name="scope">the intened to find scope pointer</param>
		/// <returns>a pair of Datum pointer and the index</returns>
		std::pair<Datum*, std::size_t> FindContainedScope(const Scope* scope);

		/// <summary>
		/// Clear the scope
		/// </summary>
		void Clear();

		/// <summary>
		/// to Set the parent to nullptr
		/// </summary>
		void Orphan();
		
		/// <summary>
		/// to compare a scope and an RTTI*
		/// </summary>
		/// <param name="rhs">the RTTI pointer you intend to compare</param>
		/// <returns>if it's equal</returns>
		bool Equals(const RTTI* rhs) const override;

		const size_t Size();

	protected:
		Scope* _parent{nullptr};
		HashMap<std::string, Datum> _hashMap;
		Vector<typename HashMap<std::string, Datum>::PairType*> _orderVector;
		virtual gsl::owner<Scope*> Clone() const;
	};

	ConcreteFactory(Scope, Scope)
}

