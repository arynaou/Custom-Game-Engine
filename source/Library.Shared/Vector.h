#pragma once
#include "pch.h"
#include "DefaultIncrement.h"
#include <type_traits>
#include <iterator>
#include <functional>
#include <cstdint>
#include <stdexcept>

namespace FieaGameEngine
{
	template <typename T>
	class Vector final
	{
	public:
		using IncrementFunctor = std::function<std::size_t(std::size_t, std::size_t)>;

		class Iterator
		{
			friend Vector;
			//template <class Iterator> class iterator_traits;
		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = value_type&;
			using pointer = value_type*;
			using iterator_category = std::bidirectional_iterator_tag;

			Iterator() = default;
			Iterator(const Iterator& other) = default;
			Iterator(Iterator&& other) = default;
			Iterator& operator=(Iterator&& other) = default;
			~Iterator() = default;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other">comparision Iterator</param>
			/// <returns>if the iterator is the same</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other">comparision Iterator</param>
			/// <returns>if the iterator is not the same</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other">the Iterator intend to copy</param>
			/// <returns>the Iterator you copy</returns>
			Iterator& operator = (const Iterator& other);

			/// <summary>
			/// Prefix increment
			/// </summary>
			/// <returns>the next index of the iterator</returns>
			Iterator& operator++();

			/// <summary>
			/// Postfix increment
			/// </summary>
			/// <returns>the next index of the itereator</returns>
			Iterator operator++(int);

			Iterator& operator--();

			Iterator& operator--(int);

			/// <summary>
			/// Dereference
			/// </summary>
			/// <returns>Get the value of the Iterator current Node</returns>
			T& operator*() const;

		private:
			Iterator(const Vector& owner, size_t index =0);

			const Vector* _owner{ nullptr };
			size_t _index{ 0 };
		};

		class ConstIterator
		{
			friend Vector;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = value_type&;
			using pointer = value_type*;
			using iterator_category = std::bidirectional_iterator_tag;

		
			ConstIterator() = default;
			ConstIterator(const Iterator& other);
			ConstIterator(const ConstIterator& other);
			ConstIterator(ConstIterator&& other) = default;
			ConstIterator& operator=(ConstIterator && other) = default;
			~ConstIterator() = default;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other">comparision const Iterator</param>
			/// <returns>if the const iterator is the same</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other">comparision const Iterator</param>
			/// <returns>if the const iterator is not the same</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other">the Iterator intend to copy</param>
			/// <returns>the Iterator you copy</returns>
			ConstIterator& operator = (const Iterator& other);

			/// <summary>
			/// Prefix increment
			/// </summary>
			/// <returns>the next index of the const itereator</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Pistfix increment
			/// </summary>
			/// <returns>the next index of the const itereator</returns>
			ConstIterator operator++(int);

			ConstIterator& operator--();

			ConstIterator& operator--(int);

			/// <summary>
			/// Dereference
			/// </summary>
			/// <returns>Get the value of the const Iterator current Node</returns>
			const T& operator*() const;

		private:
			ConstIterator(const Vector& owner, const size_t index = 0);

			const Vector* _owner{ nullptr };
			size_t _index{ 0 };
		};

		Vector(std::size_t capacity = 0, IncrementFunctor incrementFunctor = DefaultIncrement{});
		Vector(IncrementFunctor incrementFunctor);
		Vector(std::initializer_list<T> list);
		
		/// <summary>
		/// Instantiate a Vector from a existing instance
		/// </summary>
		/// <param name="other">The source instance</param>
		Vector(const Vector & other);

		/// <summary>
		/// Assign a vector
		/// </summary>
		/// <returns>The current instance (i.e. the left-hand side of the assignment)</returns>
		Vector& operator=(const Vector & other);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">the vector intend to move</param>
		/// <returns></returns>
		Vector(Vector&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">the vector intend to move</param>
		/// <returns></returns>
		Vector& operator=(Vector && rhs) noexcept;

		/// <summary>
		/// return a reference to the item at the given index.
		/// </summary>
		/// <param name="value">index</param>
		/// <returns>the value of the index in the Vector</returns>
		T& operator[](const size_t value);

		/// <summary>
		/// return a reference to the item at the given index
		/// </summary>
		/// <param name="value">index</param>
		/// <returns>the value of the index in the vector</returns>
		const T& operator[](const size_t value) const;

		~Vector();

		/// <summary>
		/// return a reference to the item at the given index.
		/// </summary>
		/// <param name="value">index</param>
		/// <returns>the value of the index in the Vector</returns>
		T& At(const size_t value);

		/// <summary>
		/// return a reference to the item at the given index
		/// </summary>
		/// <param name="value">index</param>
		/// <returns>the value of the index in the vector</returns>
		const T& At(const size_t value) const;

		/// <summary>
		/// return the first element in the Vector
		/// </summary>
		/// <returns>the first element</returns>
		T& Front();
		
		/// <summary>
		/// return the first element in the Vector
		/// </summary>
		/// <returns>the first element</returns>
		const T& Front() const;

		/// <summary>
		/// return the last element in the Vector
		/// </summary>
		/// <returns>the last element</returns>
		T& Back();

		/// <summary>
		/// return the last element in the Vector
		/// </summary>
		/// <returns>the last element</returns>
		const T& Back() const;

		/// <summary>
		/// append given item after the end of the Vector.
		/// </summary>
		/// <param name="value">append item</param>
		Iterator PushBack(const T & value);

		/// <summary>
		/// remove the last item from of the vector.
		/// </summary>
		void PopBack();

		template <typename... Args>
		Iterator EmplaceBack(Args&&... args);

		/// <summary>
		/// 
		/// </summary>
		/// <returns>the size of the vector</returns>
		const std::size_t& Size() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns>the capacity of the vector</returns>
		const std::size_t& Capacity() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns>if the vector is empty</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Clear the vector
		/// </summary>
		void Clear();

		/// <summary>
		/// take an unsigned integer indicating the capacity to reserve for the array
		/// </summary>
		/// <param name="capacity">how many capacity you need to add to the vector</param>
		void Reserve(std::size_t capacity);

		/// <summary>
		/// allows the removal of any element in the vector 
		/// </summary>
		/// <param name="value">the intend remove value</param>
		bool Remove(const T& value);
		bool Remove(const Iterator& it);
		void Remove(const Iterator& first, const Iterator& last);

		/// <summary>
		/// Find if any element is in the vector
		/// </summary>
		/// <param name="value">the intend to find element</param>
		/// <returns>the iterator if find that element</returns>
		Iterator Find(const T & value);

		/// <summary>
		/// Find if any element is in the vector
		/// </summary>
		/// <param name="value">the intend to find element</param>
		/// <returns>the iterator if find that element</returns>
		ConstIterator Find(const T& value) const;

		void Resize(std::size_t size);
		void Resize(std::size_t capacity, const T& prototype);

		/// <summary>
		/// begin of the iterator
		/// </summary>
		/// <returns>the iterator</returns>
		Iterator begin();

		/// <summary>
		/// begin of the iterator
		/// </summary>
		/// <returns>the iterator</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// begin of the iterator
		/// </summary>
		/// <returns>the iterator</returns>
		ConstIterator begin() const;

		/// <summary>
		/// the end of the iterator
		/// </summary>
		/// <returns>the iterator</returns>
		Iterator end();

		/// <summary>
		/// the end of the iterator
		/// </summary>
		/// <returns>the iterator</returns>
		ConstIterator cend() const;

		/// <summary>
		/// the end of the iterator
		/// </summary>
		/// <returns>the iterator</returns>
		ConstIterator end() const;

		void ShrinkToFit();

	private:
		std::size_t _size{ 0 };
		std::size_t _capacity{ 0 };
		T* _data{ nullptr };
		IncrementFunctor _IncrementFunctor{ DefaultIncrement{} };
	};
}
#include "Vector.inl"