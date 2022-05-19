#pragma once

#include "CppUnitTest.h"
#include "Foo.h"
#include "SList.h"
#include <string>
#include "Vector.h"
#include "HashMap.h"
#include "Datum.h"
#include "AttributedFoo.h"
#include "JsonParseMaster.h"
#include "JsonTestParseHelper.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include "GameObject.h"
#include "Action.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "Event.h"
#include "GameState.h"
#include "EventPublisher.h"

using namespace std::string_literals;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<Foo>(Foo* t)
	{
		RETURN_WIDE_STRING(t->Data());
	}

	template<>
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t); 
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);  
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	//template<>
	//inline std::wstring ToString<HashMap<Foo, Foo>::Iterator>(const HashMap<Foo,Foo>::Iterator* t)
	//{
	//	try
	//	{
	//		return ToString(t);
	//	}
	//	catch (const std::exception&)
	//	{
	//		return L"nullptr";
	//	}
	//}

	//template<>
	//inline std::wstring ToString<HashMap<Foo, Foo>::Iterator>(HashMap<Foo, Foo>::Iterator* t)
	//{
	//	try
	//	{
	//		return ToString(t);
	//	}
	//	catch (const std::exception&)
	//	{
	//		return L"nullptr";
	//	}
	//}

	//template<>
	//inline std::wstring ToString<HashMap<Foo, Foo>::Iterator>(const HashMap<Foo, Foo>::Iterator& t)
	//{
	//	try
	//	{
	//		return ToString(*t);
	//	}
	//	catch (const std::exception&)
	//	{
	//		return L"nullptr";
	//	}
	//}

	//template<>
	//inline std::wstring ToString<HashMap<Foo, Foo>::ConstIterator>(const HashMap<Foo, Foo>::ConstIterator* t)
	//{
	//	try
	//	{
	//		return ToString(t);
	//	}
	//	catch (const std::exception&)
	//	{
	//		return L"nullptr";
	//	}
	//}

	//template<>
	//inline std::wstring ToString<HashMap<Foo, Foo>::ConstIterator>(HashMap<Foo, Foo>::ConstIterator* t)
	//{
	//	try
	//	{
	//		return ToString(t);
	//	}
	//	catch (const std::exception&)
	//	{
	//		return L"nullptr";
	//	}
	//}

	//template<>
	//inline std::wstring ToString<HashMap<Foo, Foo>::ConstIterator>(const HashMap<Foo, Foo>::ConstIterator& t)
	//{
	//	try
	//	{
	//		return ToString(*t);
	//	}
	//	catch (const std::exception&)
	//	{
	//		return L"nullptr";
	//	}
	//}


	template<typename T, typename Q> inline std::wstring ToString(const HashMap<T, Q>& t) { RETURN_WIDE_STRING(t.Size()); }
	template<typename T, typename Q> inline std::wstring ToString(const HashMap<T, Q>* t) { RETURN_WIDE_STRING(t); }
	template<typename T, typename Q> inline std::wstring ToString(HashMap<T, Q>* t) { RETURN_WIDE_STRING(t); }

	template<typename T, typename Q> inline std::wstring ToString(const std::pair<T, Q>& t) 
	{ 
		try
		{
			return ToString(t.second);
		}
		catch (...)
		{
			return L"nullptr";
		}
	}
	template<typename T, typename Q> inline std::wstring ToString(const std::pair<T, Q>* t)
	{
		try
		{
			return ToString(t->second);
		}
		catch (...)
		{
			return L"nullptr";
		}
	}

	template<typename T, typename Q> inline std::wstring ToString(std::pair<T, Q>* t)
	{
		try
		{
			return ToString(t->second);
		}
		catch (...)
		{
			return L"nullptr";
		}
	}

	template<typename T, typename Q> inline std::wstring ToString(const std::pair<const T, Q>& t)
	{
		try
		{
			return ToString(t.second);
		}
		catch (...)
		{
			return L"nullptr";
		}
	}
	template<typename T, typename Q> inline std::wstring ToString(const std::pair<const T, Q>* t)
	{
		try
		{
			return ToString(t->second);
		}
		catch (...)
		{
			return L"nullptr";
		}
	}

	template<typename T, typename Q> inline std::wstring ToString(std::pair<const T, Q>* t)
	{
		try
		{
			return ToString(t->second);
		}
		catch (...)
		{
			return L"nullptr";
		}
	}
	
	template<> 
	inline std::wstring ToString<enum FieaGameEngine::DatumTypes>(const enum FieaGameEngine::DatumTypes& t)
	{
		t;
		return L"end()"s;
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		std::wstring string_value = ToString(t.x) + L" "s + ToString(t.y) + L" "s + ToString(t.z) + L" "s + ToString(t.w);
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		std::wstring string_value = ToString(t[0]) + L" "s + ToString(t[1]) + L" "s + ToString(t[2]) + L" "s + ToString(t[3]);
		RETURN_WIDE_STRING(string_value);
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum& t)
	{
		return ToString(t.Type());
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template <>
	inline std::wstring ToString<RTTI*>(RTTI** t)
	{
		RETURN_WIDE_STRING(t);
	}

	template <>
	inline std::wstring ToString<RTTI>(RTTI* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template <>
	inline std::wstring ToString<RTTI>(const RTTI* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template <>
	inline std::wstring ToString<Scope>(Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template <>
	inline std::wstring ToString<Scope*>(Scope** t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Attributed>(Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Attributed>(const Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::PairType>(const HashMap<Foo, Foo>::PairType& t)
	{
		std::wstringstream s;

		s << "(" << ToString(t.first) << ", " << ToString(t.second) << ")";

		return s.str();
	}

	template<>
	inline std::wstring ToString<JsonParseMaster>(JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster>(const JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster::SharedData>(JsonParseMaster::SharedData* t)
	{
		RETURN_WIDE_STRING(t->TypeName().c_str());
	}

	template<>
	inline std::wstring ToString<IJsonParseHelper>(IJsonParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTestParseHelper>(JsonTestParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTestParseHelper::SharedData>(JsonTestParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTestParseHelper::SharedData>(const JsonTestParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}


	template<>
	inline std::wstring ToString<JsonTableParseHelper>(JsonTableParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTableParseHelper::SharedData>(JsonTableParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTableParseHelper::SharedData>(const JsonTableParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FooFactory>(FooFactory* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FooFactory>(const FooFactory* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<GameObject>(const GameObject& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<GameObject>(GameObject* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<GameObject>(const GameObject* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Action>(const Action& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<Action>(Action* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Action>(const Action* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionIncrement>(const ActionIncrement& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<ActionIncrement>(ActionIncrement* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionIncrement>(const ActionIncrement* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionCreateAction>(const ActionCreateAction& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<ActionCreateAction>(ActionCreateAction* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionCreateAction>(const ActionCreateAction* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionDestroyAction>(const ActionDestroyAction& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<ActionDestroyAction>(ActionDestroyAction* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionDestroyAction>(const ActionDestroyAction* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionList>(const ActionList& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<ActionList>(ActionList* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionList>(const ActionList* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionListIf>(const ActionListIf& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<ActionListIf>(ActionListIf* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionListIf>(const ActionListIf* t)
	{
		RETURN_WIDE_STRING(t);
	}


	template<>
	inline std::wstring ToString<Event<Foo>>(Event<Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<EventPublisher>(EventPublisher* t)
	{
		RETURN_WIDE_STRING(t);
	}
}