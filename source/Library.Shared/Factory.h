#pragma once
#include "HashMap.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A template can produce different type of object
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class Factory
	{
	public:

		/// <summary>
		/// The constructor of Factory
		/// </summary>
		Factory() = default;
		virtual ~Factory() = default;
		Factory(const Factory& rhs) = delete;
		Factory(Factory&& rhs) = delete;
		Factory& operator=(const Factory& rhs) = delete;
		Factory& operator=(Factory&& rhs) = delete;

		/// <summary>
		/// Given a class name (string), return the associated concrete factory
		/// </summary>
		/// <param name="name">the name user wants to find</param>
		/// <returns>the concrete factory pointer</returns>
		static Factory<T>* Find(const std::string& name);

		/// <summary>
		/// Given a class name (string), return a new object of that type.
		/// </summary>
		/// <param name="name">the class name</param>
		/// <returns>the new object of that type</returns>
		static gsl::owner<T*> Create(const std::string& name);

		/// <summary>
		/// Return the address of a concrete product associated with this concrete factory class. 
		/// </summary>
		/// <returns>the new object of that type</returns>
		virtual gsl::owner<T*> Create() const = 0;

		/// <summary>
		/// return the name of the class the factory instantiates
		/// </summary>
		/// <returns>the name of the class</returns>
		virtual const std::string ClassName() const = 0;

	protected:
		static void Add(Factory<T>&);
		static void Remove(const Factory<T>&);

		inline static HashMap<std::string, Factory<T>*> _factoryHashmap;
	};
}

#define ConcreteFactory(ConcreteProductType, AbstractProductType)													\
	class ConcreteProductType##Factory : public FieaGameEngine::Factory<AbstractProductType>						\
	{																												\
	public:																											\
		ConcreteProductType##Factory() : _className(#ConcreteProductType){ Add(*this); }							\
		~ConcreteProductType##Factory() { Remove(*this); }															\
		const std::string ClassName() const override { return _className ;}											\
		gsl::owner<ConcreteProductType*> Create() const override {return new ConcreteProductType;}					\
	private:																										\
		std::string _className;																						\
	};

#include "Factory.inl"