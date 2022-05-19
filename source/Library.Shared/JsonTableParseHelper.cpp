#include "pch.h"
#include "JsonTableParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonTableParseHelper);
	RTTI_DEFINITIONS(JsonTableParseHelper::SharedData);

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::Create() const
	{
		return new JsonTableParseHelper();
	}

	JsonTableParseHelper::JsonTableParseHelper()
	{
		JsonTableParseHelper::Initialize();
	}

	void JsonTableParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();

		_parsingData = false;
	}


	bool JsonTableParseHelper::StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		JsonTableParseHelper::SharedData* customSharedData = sharedData.As<JsonTableParseHelper::SharedData>();

		if (customSharedData == nullptr)
		{
			return false;
		}

		if (_stack.IsEmpty())
		{
			_stack.Push({ "shareddata", customSharedData->GetData() });
		}

		Scope& stackScope = _stack.Top().scope;
		std::string stackKey = _stack.Top().key;
		
		if (key == "type")
		{
			DatumTypes type = _hashmap.At(value.asString());
			stackScope.Append(_stack.Top().key).SetType(type);
			_externalArrayTrackInt = 0;
		}
		else if (key == "value")
		{
			if (value.isObject())
			{
				Factory<Scope>* foundFactory = Factory<Scope>::Find(value["class"].asString());
				Scope* newScope = nullptr;
				_externalArrayTrackInt = 0;
				if (foundFactory != nullptr)
				{
					newScope = foundFactory->Create();
				}
				else
				{
					newScope = Factory<Scope>::Create("Scope");
				}
				_stack.Top().scope.Adopt(*newScope, stackKey);
				_stack.Push({ stackKey, *(newScope) });
			}
			else
			{
				Datum& datum = stackScope[stackKey];
				if (!datum.IsInternal())
				{
					datum.SetFromString(value.asString(), _externalArrayTrackInt);
					++_externalArrayTrackInt;
				}
				else
				{
					datum.PushBackFromString(value.asString());
				}
			}
		}
		else
		{
			_stack.Push({ key,stackScope });
		}

		return true;
	}

	bool JsonTableParseHelper::EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		JsonTableParseHelper::SharedData* customSharedData = sharedData.As<JsonTableParseHelper::SharedData>();

		if (customSharedData == nullptr)
		{
			return false;
		}

		if ((key != "type" && key != "value") || value.isObject())
		{
			_stack.Pop();
		}
		
		return true;
	}

	void JsonTableParseHelper::SharedData::Initialize()
	{
		JsonParseMaster::SharedData::Initialize();
		_data = new GameObject();
	}

	void JsonTableParseHelper::SharedData::CleanUp()
	{
		delete _data;
	}

	JsonTableParseHelper::SharedData::SharedData() : JsonParseMaster::SharedData()
	{
		Initialize();
	}

	JsonTableParseHelper::SharedData::~SharedData()
	{
		CleanUp();
	}

	gsl::owner<JsonTableParseHelper::SharedData*> JsonTableParseHelper::SharedData::Create() const
	{
		return new JsonTableParseHelper::SharedData();
	}

	JsonTableParseHelper::~JsonTableParseHelper()
	{
	}

	GameObject& JsonTableParseHelper::SharedData::GetData()
	{
		return *_data;
	}
}