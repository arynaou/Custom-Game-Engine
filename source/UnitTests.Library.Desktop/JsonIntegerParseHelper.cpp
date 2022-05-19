#include "pch.h"
#include "JsonIntegerParseHelper.h"

using namespace FieaGameEngine;

namespace UnitTests
{
	RTTI_DEFINITIONS(JsonIntegerParseHelper);
	RTTI_DEFINITIONS(JsonIntegerParseHelper::SharedData);

	gsl::owner<IJsonParseHelper*> JsonIntegerParseHelper::Create() const
	{
		return new JsonIntegerParseHelper();
	}

	void JsonIntegerParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		_parsingData = false;
	}

	bool JsonIntegerParseHelper::StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		JsonIntegerParseHelper::SharedData* customSharedData = sharedData.As<JsonIntegerParseHelper::SharedData>();
		if (customSharedData == nullptr)
		{
			return false;
		}

		if (key != IntegerKey)
		{
			return false;
		}

		if (_parsingData)
		{
			throw std::runtime_error("Received another start element while already parsing an integer.");
		}

		if (value.isArray()) return false;
		
		_parsingData = true;


		customSharedData->Data.PushBack(value.asInt());

		return true;
	}

	bool JsonIntegerParseHelper::EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& key, const Json::Value&)
	{
		JsonIntegerParseHelper::SharedData* customSharedData = sharedData.As<JsonIntegerParseHelper::SharedData>();
		if (customSharedData == nullptr || key != IntegerKey || _parsingData == false)
		{
			return false;
		}
		
		_parsingData = false;

		return true;
	}

	void JsonIntegerParseHelper::SharedData::Initialize()
	{
		JsonParseMaster::SharedData::Initialize();
	}

	gsl::owner<JsonIntegerParseHelper::SharedData*> JsonIntegerParseHelper::SharedData::Create() const
	{
		return new JsonIntegerParseHelper::SharedData();
	}
}