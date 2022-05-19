#pragma once
#include "Factory.h"

namespace FieaGameEngine
{
	template <typename T>
	Factory<T>* Factory<T>::Find(const std::string& name)
	{
		typename HashMap<std::string, Factory<T>*>::Iterator find = _factoryHashmap.Find(name);
		if (find != _factoryHashmap.end())
		{
			return (*find).second;
		}
		return nullptr;
	}

	template <typename T>
	gsl::owner<T*> Factory<T>::Create(const std::string& name)
	{
		Factory<T>* find = Find(name);
		if (find != nullptr)
		{
			return (*find).Create();
		}

		return nullptr;
	}

	template <typename T>
	void Factory<T>::Add(Factory<T>& factory)
	{
		bool isInserted = (_factoryHashmap.Insert(std::make_pair(factory.ClassName(), &factory))).second;

		if (!isInserted)
		{
			throw std::exception("you can't add another factory");
		}
	}

	template <typename T>
	void Factory<T>::Remove(const Factory<T>& factory)
	{
		_factoryHashmap.Remove(factory.ClassName());
	}
}