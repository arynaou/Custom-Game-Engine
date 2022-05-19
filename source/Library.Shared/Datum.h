#pragma once
#include "pch.h"
#include "RTTI.h"
#include "glm/glm.hpp"

namespace FieaGameEngine
{
	class RTTI;
	class Scope;

	enum class DatumTypes
	{
		Unknown,
		Integer,
		Float,
		Vectors,
		Matrix,
		String,
		Pointer,
		Table,
		Reference,

		Begin = Integer,
		End = Reference
	};

	class Datum
	{
	private:
		union DatumValues final
		{
			RTTI** r;
			std::int32_t* i;
			float* f;
			glm::vec4* v;
			glm::mat4* m;
			std::string* s;
			Scope** sc;
			void* vp{ nullptr };
		};
	
	public:

		friend Scope;
		friend class Attributed;

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="type">which type of things assign to Datum</param>
		/// <param name="capacity">how large the datum you need</param>
		Datum(DatumTypes type = DatumTypes::Unknown, std::size_t capacity = 0);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">the Datun intend to copy</param>
		Datum(const Datum& other);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">the int intend to copy</param>
		Datum(const std::int32_t& other);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">the float intend to copy</param>
		Datum(const float& other);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">the Vector4 intend to copy</param>
		Datum(const glm::vec4& other);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">the Matrix4 intend to copy</param>
		Datum(const glm::mat4& other);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">the string intend to copy</param>
		Datum(const std::string other);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="other">the RTTI pointer intend to copy</param>
		Datum(RTTI* other);

		Datum(Scope* other);

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="other">the Datum intend to copy</param>
		Datum& operator = (const Datum& other);

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="other">the int intend to copy</param>
		Datum& operator = (const std::int32_t& other);

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="other">the float intend to copy</param>
		Datum& operator = (const float& other);


		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="other">the vector intend to copy</param>
		Datum& operator = (const glm::vec4& other);

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="other">the matrix intend to copy</param>
		Datum& operator = (const glm::mat4& other);

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="other">the string intend to copy</param>
		Datum& operator = (const std::string& other);

		/// <summary>
		/// Copy Assignment
		/// </summary>
		/// <param name="other">the RTTI pointer intend to copy</param>
		Datum& operator = (RTTI* other);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		Datum(Datum&& rhs) noexcept;

		/// <summary>
		/// Move assignmrnt
		/// </summary>
		/// <param name="other"></param>
		/// <returns>Datum</returns>
		Datum& operator=(Datum&& other) noexcept;
		
		~Datum();

		/// <summary>
		/// Get Type
		/// </summary>
		/// <returns>return type</returns>
		const DatumTypes Type() const;

		/// <summary>
		/// Set Type
		/// </summary>
		/// <param name="type">the datum type</param>
		void SetType(DatumTypes type);

		/// <summary>
		/// the size of the datum
		/// </summary>
		/// <returns>size</returns>
		std::size_t Size() const;

		/// <summary>
		/// the capacity of the datum
		/// </summary>
		/// <returns>capacity</returns>
		std::size_t Capacity() const;

		/// <summary>
		/// take an unsigned integer indicating the capacity to reserve for the array,
		/// </summary>
		/// <param name="capacity">capacity</param>
		void Reserve(std::size_t capacity);

		/// <summary>
		/// set number of values and reserve memory if needed
		/// </summary>
		/// <param name="size">integer number</param>
		void Resize(std::size_t size);

		/// <summary>
		/// Clear the datum
		/// </summary>
		void Clear();

		/// <summary>
		/// Given an array and the number of elements in it, assign the internal values array to the given array, and the size and capacity to the given number of elements.
		/// </summary>
		/// <param name="other">integer</param>
		/// <param name="size">set size</param>
		void SetStorage(std::int32_t* const other, std::size_t size);

		/// <summary>
		/// Given an array and the number of elements in it, assign the internal values array to the given array, and the size and capacity to the given number of elements.
		/// </summary>
		/// <param name="other">float</param>
		/// <param name="size">set size</param>
		void SetStorage(float* const other, std::size_t size);


		/// <summary>
		/// Given an array and the number of elements in it, assign the internal values array to the given array, and the size and capacity to the given number of elements.
		/// </summary>
		/// <param name="other">Vector</param>
		/// <param name="size">set size</param>
		void SetStorage(glm::vec4* const other, std::size_t size);

		/// <summary>
		/// Given an array and the number of elements in it, assign the internal values array to the given array, and the size and capacity to the given number of elements.
		/// </summary>
		/// <param name="other">Matrix</param>
		/// <param name="size">set size</param>
		void SetStorage(glm::mat4* const other, std::size_t size);

		/// <summary>
		/// Given an array and the number of elements in it, assign the internal values array to the given array, and the size and capacity to the given number of elements.
		/// </summary>
		/// <param name="other">String</param>
		/// <param name="size">set size</param>
		void SetStorage(std::string* const other, std::size_t size);

		/// <summary>
		/// Given an array and the number of elements in it, assign the internal values array to the given array, and the size and capacity to the given number of elements.
		/// </summary>
		/// <param name="other">RTTI pointer</param>
		/// <param name="size">set size</param>
		void SetStorage(RTTI** const other, std::size_t size);

		Scope& operator[](std::size_t index);

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">Datum</param>
		/// <returns>if is equal</returns>
		bool operator==(const Datum& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">int</param>
		/// <returns>if is equal</returns>
		bool operator==(const std::int32_t& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">float</param>
		/// <returns>if is equal</returns>
		bool operator==(const float& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">vector</param>
		/// <returns>if is equal</returns>
		bool operator==(const glm::vec4& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">matrix</param>
		/// <returns>if is equal</returns>
		bool operator==(const glm::mat4 & other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">string</param>
		/// <returns>if is equal</returns>
		bool operator==(const std::string& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">RTTI pointer</param>
		/// <returns>if is equal</returns>
		bool operator==(const RTTI* other) const;

		//bool operator==(const Scope* other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">Datum</param>
		/// <returns>if is not equal</returns>
		bool operator!=(const Datum& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">int</param>
		/// <returns>if is not equal</returns>
		bool operator!=(const std::int32_t& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">float</param>
		/// <returns>if is not equal</returns>
		bool operator!=(const float& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">Vector</param>
		/// <returns>if is not equal</returns>
		bool operator!=(const glm::vec4& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">matrix</param>
		/// <returns>if is not equal</returns>
		bool operator!=(const glm::mat4& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">String</param>
		/// <returns>if is not equal</returns>
		bool operator!=(const std::string& other) const;

		/// <summary>
		/// Comparision operator
		/// </summary>
		/// <param name="other">RTTI pointer</param>
		/// <returns>if is not equal</returns>
		bool operator!=(const RTTI* other) const;

		//bool operator!=(const Scope* other) const;

		/// <summary>
		///  Given a value and an optional index (default is 0), assign the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">the int value intend to assign</param>
		/// <param name="index">the index you want to set</param>
		void Set(const std::int32_t& value, std::size_t index = 0);


		/// <summary>
		///  Given a value and an optional index (default is 0), assign the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">the float value intend to assign</param>
		/// <param name="index">the index you want to set</param>
		void Set(const float& value, std::size_t index = 0);


		/// <summary>
		///  Given a value and an optional index (default is 0), assign the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">the vector value intend to assign</param>
		/// <param name="index">the index you want to set</param>
		void Set(const glm::vec4& value, std::size_t index = 0);


		/// <summary>
		///  Given a value and an optional index (default is 0), assign the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">the matrix value intend to assign</param>
		/// <param name="index">the index you want to set</param>
		void Set(const glm::mat4& value, std::size_t index = 0);


		/// <summary>
		///  Given a value and an optional index (default is 0), assign the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">the string value intend to assign</param>
		/// <param name="index">the index you want to set</param>
		void Set(const std::string& value, std::size_t index = 0);


		/// <summary>
		///  Given a value and an optional index (default is 0), assign the appropriate element in the values array to the given value.
		/// </summary>
		/// <param name="value">the RTTI pointer value intend to assign</param>
		/// <param name="index">the index you want to set</param>
		void Set(RTTI* value, std::size_t index = 0);

		//TODO change to Scope&
		void Set(Scope* value, std::size_t index = 0);

		/// <summary>
		/// Given an optional index (default is 0), return the appropriate value from the values array.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="index">index</param>
		/// <returns></returns>
		template <typename T>
		T& Get(std::size_t index=0);

		/// <summary>
		/// Given an optional index (default is 0), return the appropriate value from the values array.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="index">index</param>
		/// <returns></returns>
		template <typename T>
		const T& Get(std::size_t index = 0) const;
		
		/// <summary>
		/// given an STL string and an optional index (default 0) assign the appropriate element in the values array to the given value 
		/// </summary>
		/// <param name="value">the STL string</param>
		/// <param name="index">the index you want to set</param>
		void SetFromString(std::string value, std::size_t index);

		void PushBackFromString(std::string value);

		/// <summary>
		///  given an optional index (default is 0) return an STL string representing the appropriate element in the values array.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="index">the index you want to set</param>
		/// <returns>STL string</returns>
		template <typename T>
		std::string ToString(std::size_t index=0);

		/// <summary>
		/// append given item after the end of datum.
		/// </summary>
		/// <param name="value">append item</param>
		void PushBack(const std::int32_t& value);
		
		/// <summary>
		/// append given item after the end of datum.
		/// </summary>
		/// <param name="value">append item</param>
		void PushBack(const float& value);
		
		/// <summary>
		/// append given item after the end of datum.
		/// </summary>
		/// <param name="value">append item</param>
		void PushBack(const glm::vec4& value);
		
		/// <summary>
		/// append given item after the end of datum.
		/// </summary>
		/// <param name="value">append item</param>
		void PushBack(const glm::mat4& value);
		
		/// <summary>
		/// append given item after the end of datum.
		/// </summary>
		/// <param name="value">append item</param>
		void PushBack(const std::string& value);
		
		/// <summary>
		/// append given item after the end of datum.
		/// </summary>
		/// <param name="value">append item</param>
		void PushBack(RTTI* value);


		//TODO change to Scope&
		void PushBack(Scope* value);

		/// <summary>
		/// remove the last item from of the list.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Shrink the Capacity to Size
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Return the first element in datum
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template <typename T>
		T& Front();

		/// <summary>
		/// Return the first element in datum
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template <typename T>
		const T& Front() const;

		/// <summary>
		/// Return the last element in datum
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template <typename T>
		T& Back();

		/// <summary>
		/// Return the last element in datum
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template <typename T>
		const T& Back() const;

		/// <summary>
		/// Remove certain value in Datum
		/// </summary>
		/// <param name="value">the thing you want to remove</param>
		/// <returns>if you remove the item</returns>
		bool Remove(const std::int32_t& value);

		/// <summary>
		/// Remove certain value in Datum
		/// </summary>
		/// <param name="value">the thing you want to remove</param>
		/// <returns>if you remove the item</returns>
		bool Remove(const float& value);

		/// <summary>
		/// Remove certain value in Datum
		/// </summary>
		/// <param name="value">the thing you want to remove</param>
		/// <returns>if you remove the item</returns>
		bool Remove(const glm::vec4& value);

		/// <summary>
		/// Remove certain value in Datum
		/// </summary>
		/// <param name="value">the thing you want to remove</param>
		/// <returns>if you remove the item</returns>
		bool Remove(const glm::mat4& value);

		/// <summary>
		/// Remove certain value in Datum
		/// </summary>
		/// <param name="value">the thing you want to remove</param>
		/// <returns>if you remove the item</returns>
		bool Remove(const std::string& value);

		/// <summary>
		/// Remove certain value in Datum
		/// </summary>
		/// <param name="value">the thing you want to remove</param>
		/// <returns>if you remove the item</returns>
		bool Remove(const RTTI* value);

		bool Remove(const Scope* value);
		
		/// <summary>
		/// Remove the item in the certain index
		/// </summary>
		/// <param name="index"></param>
		void RemoveAt(const std::size_t index);

		/// <summary>
		/// Find the given value
		/// </summary>
		/// <param name="value"></param>
		/// <returns>return index, if it's not found return size</returns>
		std::size_t Find(const std::int32_t& value) const;
		
		/// <summary>
		/// Find the given value
		/// </summary>
		/// <param name="value"></param>
		/// <returns>return index, if it's not found return size</returns>
		std::size_t Find(const float& value) const;

		/// <summary>
		/// Find the given value
		/// </summary>
		/// <param name="value"></param>
		/// <returns>return index, if it's not found return size</returns>
		std::size_t Find(const glm::vec4& value) const;

		/// <summary>
		/// Find the given value
		/// </summary>
		/// <param name="value"></param>
		/// <returns>return index, if it's not found return size</returns>
		std::size_t Find(const glm::mat4& value) const;

		/// <summary>
		/// Find the given value
		/// </summary>
		/// <param name="value"></param>
		/// <returns>return index, if it's not found return size</returns>
		std::size_t Find(const std::string& value) const;

		/// <summary>
		/// Find the given value
		/// </summary>
		/// <param name="value"></param>
		/// <returns>return index, if it's not found return size</returns>
		std::size_t Find(const RTTI* value) const;

		//TODO change to Scope&
		std::size_t Find(const Scope* value) const;

		/// <summary>
		/// if datum is Internal
		/// </summary>
		/// <returns>if it's internal</returns>
		const bool IsInternal() const;

	private:
		DatumValues _data;
		std::size_t _capacity{ 0 };
		std::size_t _size{ 0 };
		DatumTypes _type{ DatumTypes::Unknown };
		bool _isInternal{ true };

		static const std::size_t DatumTypeSizes[static_cast<std::size_t>(DatumTypes::End)];

		void SetStorage(void* const other, std::size_t size, DatumTypes type);
	};
}