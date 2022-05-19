#include "pch.h"
#include "JsonTestParseHelper.h"

using namespace FieaGameEngine;

namespace UnitTests
{
	RTTI_DEFINITIONS(JsonTestParseHelper);
	RTTI_DEFINITIONS(JsonTestParseHelper::SharedData);

	gsl::owner<IJsonParseHelper*> JsonTestParseHelper::Create() const
	{
		return new JsonTestParseHelper();
	}

	void JsonTestParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		InitializeCalled = true;
		StartHandlerCount = 0;
		EndHandlerCount = 0;
	}

	bool JsonTestParseHelper::StartHandler(JsonParseMaster::SharedData& sharedData, const std::string&, const Json::Value&)
	{
		JsonTestParseHelper::SharedData* customSharedData = sharedData.As<JsonTestParseHelper::SharedData>();
		if (customSharedData == nullptr)
		{
			return false;
		}
		
		++StartHandlerCount;

		if (customSharedData->Depth() > customSharedData->MaxDepth)
		{
			customSharedData->MaxDepth = customSharedData->Depth();
		}

		return true;
	}

	bool JsonTestParseHelper::EndHandler(JsonParseMaster::SharedData& sharedData, const std::string&, const Json::Value&)
	{
		JsonTestParseHelper::SharedData* customSharedData = sharedData.As<JsonTestParseHelper::SharedData>();
		if (customSharedData == nullptr)
		{
			return false;
		}

		++EndHandlerCount;

		return true;
	}

	void JsonTestParseHelper::SharedData::Initialize()
	{
		JsonParseMaster::SharedData::Initialize();
		MaxDepth = 0;
	}

	gsl::owner<JsonTestParseHelper::SharedData*> JsonTestParseHelper::SharedData::Create() const
	{
		return new JsonTestParseHelper::SharedData();
	}
}