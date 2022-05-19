#pragma once
#include "IJsonParseHelper.h"
#include "JsonParseMaster.h"
#include "RTTI.h"

namespace UnitTests
{
	class JsonIntegerParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntegerParseHelper, FieaGameEngine::IJsonParseHelper)

	public:
		class SharedData final : public FieaGameEngine::JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FieaGameEngine::JsonParseMaster::SharedData)
			
		public:
			/// <summary>
			/// Initialize the shared data
			/// </summary>
			virtual void Initialize() override;

			/// <summary>
			/// Create an instance of the helper's shrareData
			/// </summary>
			/// <returns></returns>
			virtual gsl::owner<SharedData*> Create() const override;

			/// <summary>
			/// a vector of data (type of int)
			/// </summary>
			FieaGameEngine::Vector<std::int32_t> Data;
		};

		/// <summary>
		/// Create the instance of the helper
		/// </summary>
		/// <returns>the pointer of the helper</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const override;

		/// <summary>
		/// Initialize the helper
		/// </summary>
		virtual void Initialize() override;

		/// <summary>
		/// Try to attempt the given key and value, if it can handle, handle it and return true, if it cannot return false
		/// </summary>
		/// <param name="sharedData">the shared data reference</param>
		/// <param name="key">the string of key</param>
		/// <param name="value">the Json Value of value</param>
		/// <returns>if it handle the pair or not</returns>
		virtual bool StartHandler(FieaGameEngine::JsonParseMaster::SharedData& sharedData, const std::string& key, const Json::Value& value) override;

		/// <summary>
		/// once the handler handle the pair value, it will be called and end the handle
		/// </summary>
		/// <param name="sharedData">the shared data reference</param>
		/// <param name="key">the string of the key</param>
		/// <returns>if it handle the pair or not</returns>
		virtual bool EndHandler(FieaGameEngine::JsonParseMaster::SharedData& sharedData, const std::string& key, const Json::Value& value) override;

	private:
		inline static const std::string IntegerKey = "integer";
		bool _parsingData = false;
	};
}