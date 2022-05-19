#pragma once
#include "RTTI.h"
#include "json/json.h"
#include "JsonParseMaster.h"

namespace FieaGameEngine
{
	class IJsonParseHelper: public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI);

	public:
		/// <summary>
		/// Initialize the Json Parse Helper
		/// </summary>
		virtual void Initialize();

		virtual void CleanUp();

		/// <summary>
		/// Try to attempt the given key and value, if it can handle, handle it and return true, if it cannot return false
		/// </summary>
		/// <param name="sharedData">the shared data reference</param>
		/// <param name="key">the string of key</param>
		/// <param name="value">the Json Value of value</param>
		/// <returns>if it handle the pair or not</returns>
		virtual bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& key, const Json::Value& value) = 0;

		/// <summary>
		/// once the handler handle the pair value, it will be called and end the handle
		/// </summary>
		/// <param name="sharedData">the shared data reference</param>
		/// <param name="key">the string of the key</param>
		/// <returns>if it handle the pair or not</returns>
		virtual bool EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& key, const Json::Value& value)=0;

		/// <summary>
		/// Create a json parse helper
		/// </summary>
		/// <returns>a json parse helper pointer</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0;
	};
}