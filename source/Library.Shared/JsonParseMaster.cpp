#include "pch.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseMaster::SharedData)

	void JsonParseMaster::SharedData::Initialize()
	{
		_depth = 0;
	}

	void JsonParseMaster::SharedData::CleanUp()
	{
	}

	const std::size_t JsonParseMaster::SharedData::Depth()
	{
		return _depth;
	}

	void JsonParseMaster::SharedData::SetJsonParseMaster(JsonParseMaster* master)
	{
		_jsonParseMaster = master;
	}

	JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster()
	{
		return _jsonParseMaster;
	}

	const JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster() const
	{
		return _jsonParseMaster;
	}

	void JsonParseMaster::SharedData::IncrementDepth()
	{
		++_depth;
	}

	void JsonParseMaster::SharedData::DecrementDepth()
	{
		assert(_depth > 0);
		--_depth;
	}

	JsonParseMaster::JsonParseMaster(SharedData& sharedData)
	{
		sharedData.SetJsonParseMaster(this);
		_data = &sharedData;
	}

	JsonParseMaster::~JsonParseMaster()
	{
		Clean();
	}

	JsonParseMaster::JsonParseMaster(JsonParseMaster&& rhs) noexcept:
		 _jsonHelpers(std::move(rhs._jsonHelpers)), _data(rhs._data), isClone(rhs.isClone)
	{
		rhs.GetSharedData()._jsonParseMaster = this;

		rhs._jsonHelpers.Clear();
		rhs._data = nullptr;
		rhs.isClone = false;
	}

	JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clean();

			_jsonHelpers = std::move(rhs._jsonHelpers);
			_data = rhs._data;
			isClone = rhs.isClone;

			rhs.GetSharedData()._jsonParseMaster = this;

			rhs._jsonHelpers.Clear();
			rhs._data = nullptr;
			rhs.isClone = false;
		}
		return *this;
	}

	gsl::owner<JsonParseMaster*> JsonParseMaster::Clone() const
	{
		gsl::owner<SharedData*> newSharedData = _data->Create();
		JsonParseMaster* newParseMaster = new JsonParseMaster(*newSharedData);

		for (std::size_t i = 0; i < _jsonHelpers.Size(); ++i)
		{
			newParseMaster->_jsonHelpers.PushBack(_jsonHelpers[i]->Create());
		}

		newParseMaster->isClone = true;

		return newParseMaster;
	}

	bool JsonParseMaster::AddHelper(IJsonParseHelper& parseHelper)
	{
		if (isClone)
		{
			throw std::exception("Cannot Add Helper to a clone Master!");
		}

		for (const IJsonParseHelper* _jsonHelper : _jsonHelpers)
		{
			if (_jsonHelper->TypeIdInstance() == parseHelper.TypeIdInstance())
			{
				throw std::exception("Already have this type of helper!");
			}
		}

		_jsonHelpers.PushBack(std::move(&parseHelper));
		return true;
	}

	bool JsonParseMaster::RemoveHelper(IJsonParseHelper& parseHelper)
	{
		if (isClone)
		{
			throw std::exception("Cannot Remove Helper to a clone Master!");
		}

		_jsonHelpers.Remove(&parseHelper);

		for (const IJsonParseHelper* _jsonHelper : _jsonHelpers)
		{
			if (_jsonHelper->TypeIdInstance() == parseHelper.TypeIdInstance())
			{
				_jsonHelpers.Remove(&parseHelper);
				return true;
			}
		}
		return false;
	}

	void JsonParseMaster::Parse(const std::string& data)
	{
		std::istringstream stream(data);
		Parse(stream);
	}

	void JsonParseMaster::ParseFromFile(const std::string& fileName)
	{
		std::ifstream file(fileName, std::ifstream::binary);
		if (!file.good())
		{
			throw std::exception("the file is not exist!");
		}
		Parse(file);
	}

	void JsonParseMaster::Parse(std::istream& data)
	{
		Json::Value root;
		data >> root;
		ParseMembers(root);
	}

	typename JsonParseMaster::SharedData& JsonParseMaster::GetSharedData()
	{
		return *_data;
	}

	const typename JsonParseMaster::SharedData& JsonParseMaster::GetSharedData() const
	{
		return *_data;
	}

	void JsonParseMaster::SetSharedData(SharedData * shareData)
	{
		if (isClone)
		{
			throw std::exception("Cannot Set SharedData to a clone Master!");
		}

		_data = shareData;
	}

	void JsonParseMaster::ParseMembers(const Json::Value& data)
	{
		_data->IncrementDepth();
		for (auto const& key : data.getMemberNames())
		{
			Parse(key, data[key]);
		}
		_data->DecrementDepth();
	}

	void JsonParseMaster::Parse(const std::string& key, const Json::Value& value)
	{
		if (value.isArray())
		{
			//HelperHandler(key, value);
			for (auto const& itr : value)
			{
				HelperHandler(key, itr);
			}
		}
		else
		{
			HelperHandler(key, value);
		}
	}

	Vector<IJsonParseHelper*> JsonParseMaster::Helpers() const
	{
		return _jsonHelpers;
	}

	const bool JsonParseMaster::IsClone() const 
	{
		return isClone;
	}

	void JsonParseMaster::Initialize()
	{
		for (std::size_t i = 0; i < _jsonHelpers.Size(); ++i)
		{
			_jsonHelpers[i]->Initialize();
		}
	}

	void JsonParseMaster::HelperHandler(const std::string& key, const Json::Value& value)
	{
		for (std::size_t i = 0; i < _jsonHelpers.Size(); ++i)
		{
			if (_jsonHelpers[i]->StartHandler(*_data, key, value))
			{
				if (value.isObject())
				{
					ParseMembers(value);
				}
				_jsonHelpers[i]->EndHandler(*_data, key, value);
				break;
			}
		}
	}

	void JsonParseMaster::Clean()
	{
		if (isClone)
		{
			for (std::size_t i = 0; i < _jsonHelpers.Size(); ++i)
			{
				delete _jsonHelpers[i];
			}
			delete _data;
		}
	}
}