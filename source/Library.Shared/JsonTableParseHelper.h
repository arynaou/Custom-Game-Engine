#pragma once
#include "IJsonParseHelper.h"
#include "JsonParseMaster.h"
#include "RTTI.h"
#include "Attributed.h"
#include "Stack.h"
#include "Factory.h"
#include "GameObject.h"

namespace FieaGameEngine
{
	class JsonTableParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, FieaGameEngine::IJsonParseHelper)

	public:
		struct Context { std::string key; Scope& scope; };

		class SharedData final : public FieaGameEngine::JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FieaGameEngine::JsonParseMaster::SharedData)

		public:
			/// <summary>
			/// Constructor of SharedData
			/// </summary>
			SharedData();

			/// <summary>
			/// Destructor of SharedData
			/// </summary>
			virtual ~SharedData() override;

			/// <summary>
			/// Initialize the shared data
			/// </summary>
			virtual void Initialize() override;

			virtual void CleanUp() override;

			/// <summary>
			/// Create an instance of the helper's shrareData
			/// </summary>
			/// <returns></returns>
			virtual gsl::owner<SharedData*> Create() const override;

			GameObject& GetData();

		protected:
			/// <summary>
			/// a Game Object pointer
			/// </summary>
			GameObject* _data{ nullptr };
		};

		/// <summary>
		/// Constructor of Helper
		/// </summary>
		JsonTableParseHelper();

		~JsonTableParseHelper() override;

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
		inline static HashMap<std::string, DatumTypes> _hashmap {
			{"integer", DatumTypes::Integer},
			{"float", DatumTypes::Float},
			{"vector", DatumTypes::Vectors},
			{"matrix", DatumTypes::Matrix},
			{"string", DatumTypes::String},
			{"table", DatumTypes::Table}
		};
		Stack<Context> _stack;

		bool _parsingData = false;
		std::size_t _externalArrayTrackInt = 0;
	};
}