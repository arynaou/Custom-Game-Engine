#pragma once
#include <gsl/gsl>
#include "json/json.h"
#include "RTTI.h"
#include "Vector.h"

namespace FieaGameEngine
{
	class IJsonParseHelper;

	class JsonParseMaster final
	{
	public:
		class SharedData : public RTTI
		{
			friend JsonParseMaster;

			RTTI_DECLARATIONS(SharedData, RTTI)
		public:
			/// <summary>
			/// Initialize the Parse Master
			/// </summary>
			virtual void Initialize();

			virtual void CleanUp();

			/// <summary>
			/// Create a Shared Data pointer
			/// </summary>
			/// <returns>Shared data pointer</returns>
			virtual gsl::owner<SharedData*> Create() const =0;
			
			/// <summary>
			/// Return the depth of the current nested depth
			/// </summary>
			/// <returns>the unsigned integer of the depth</returns>
			const std::size_t Depth();

			virtual ~SharedData() = default;

			/// <summary>
			/// Get the associated Json Parse Master
			/// </summary>
			/// <returns>Json Parse Master pointer</returns>
			JsonParseMaster* GetJsonParseMaster();

			/// <summary>
			/// Get the associated Json Parse Master
			/// </summary>
			/// <returns>Json Parse Master pointer</returns>
			const JsonParseMaster* GetJsonParseMaster() const;
		
		private:
			void SetJsonParseMaster(JsonParseMaster* master);

			void IncrementDepth();
			void DecrementDepth();

			std::size_t _depth {0};
			JsonParseMaster* _jsonParseMaster{nullptr};
		};

	public:
		/// <summary>
		/// Constructor of Json Parse Master
		/// </summary>
		JsonParseMaster() = default;

		/// <summary>
		/// Constructor Json Parse Master with an associate sharedData
		/// </summary>
		/// <param name="sharedData">the intend associated sharedData</param>
		JsonParseMaster(SharedData& sharedData);

		~JsonParseMaster();
		
		/// <summary>
		/// the move constructor for Json Parse Master, will do the shallow copy
		/// </summary>
		/// <param name="">the intended moved Json Parse Master</param>
		JsonParseMaster(JsonParseMaster&&) noexcept;

		/// <summary>
		/// the move constructor for Json Parse Master, will do the shallow copy
		/// </summary>
		/// <param name="">the intended moved Json Parse Master</param>
		/// <returns>the reference of the Json Parse Master</returns>
		JsonParseMaster& operator=(JsonParseMaster&&) noexcept;

		/// <summary>
		/// Clone the Json Parse Master
		/// </summary>
		/// <returns>a Json Parse Master pointer</returns>
		virtual gsl::owner<JsonParseMaster*> Clone() const;
		
		/// <summary>
		/// Add a helper to the Json Parse Master
		/// </summary>
		/// <param name="parseHelper">the helper reference</param>
		/// <returns>if it is successfuly added</returns>
		bool AddHelper(IJsonParseHelper& parseHelper);

		/// <summary>
		/// Remove a helper to the Json Parse Master
		/// </summary>
		/// <param name="parseHelper">the helper reference</param>
		/// <returns>if it is successfuly removed</returns>
		bool RemoveHelper(IJsonParseHelper& parseHelper);

		/// <summary>
		/// Deserialize from a string
		/// </summary>
		/// <param name="data">a std::string</param>
		void Parse(const std::string& data);

		/// <summary>
		/// Deserialize from a json file
		/// </summary>
		/// <param name="fileName">the file name of the json file</param>
		void ParseFromFile(const std::string& fileName);

		/// <summary>
		/// Deserialize from std::istream
		/// </summary>
		/// <param name="data">a std::istream</param>
		void Parse(std::istream& data);

		/// <summary>
		/// Get the associated Shared Data
		/// </summary>
		/// <returns>the reference of the SharedData</returns>
		SharedData& GetSharedData();

		/// <summary>
		/// Get the associated Shared Data
		/// </summary>
		/// <returns>the reference of the SharedData</returns>
		const SharedData& GetSharedData() const;

		/// <summary>
		/// Set the associated Shared Data
		/// </summary>
		/// <param name="shareData">the pointer of the SharedData</param>
		void SetSharedData(SharedData* shareData);

		/// <summary>
		/// Get the Vector of the helpers
		/// </summary>
		/// <returns>a vector of the Json Parse Helper</returns>
		Vector<IJsonParseHelper*> Helpers() const;

		/// <summary>
		/// return if this parse Master is clone
		/// </summary>
		/// <returns>if it's clone or not</returns>
		const bool IsClone() const;
		
		/// <summary>
		/// Initialize this parse master
		/// </summary>
		virtual void Initialize();


	private:
		void HelperHandler(const std::string& key, const Json::Value& value);
		void ParseMembers(const Json::Value& data);
		void Parse(const std::string& key, const Json::Value& value);
		void Clean();

		Vector<IJsonParseHelper*> _jsonHelpers;
		SharedData* _data{nullptr};
		bool isClone{ false };
	};
}