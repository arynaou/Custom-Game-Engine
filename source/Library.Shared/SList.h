#pragma once

#include "pch.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A singly-linked list.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class SList final
	{
	private:
		struct Node final
		{
			T Data;
			Node* Next;

			Node(const T& data, Node* next = nullptr);

			template<typename... Args>
			Node(Node* next, Args&&... arg);
		};

	public:
		class Iterator
		{
			friend SList;

		public:
			Iterator() = default;
			Iterator(const Iterator& other) = default;
			Iterator& operator=(const Iterator& other) = default;
			Iterator(Iterator&& other) = default;
			Iterator& operator=(Iterator&& other) = default;
			~Iterator() = default;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other"></param>
			/// <returns>if the two iterator point to same nodes</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other"></param>
			/// <returns>if the two iterator point to different nodes</returns>
			bool operator!=(const Iterator& other) const;
			
			/// <summary>
			/// 
			/// </summary>
			/// <returns>the next node of the itereator</returns>
			Iterator& operator++();

			/// <summary>
			/// 
			/// </summary>
			/// <returns>the next node of the itereator</returns>
			Iterator operator++(int);

			/// <summary>
			/// dereference the Iterator
			/// </summary>
			/// <returns> returns a const T reference </returns>
			T& operator*() const;

		private:
			Iterator(const SList& owner, Node* node = nullptr);

			Node* _node{ nullptr };
			const SList* _owner{nullptr};
		};

		class ConstIterator
		{
			friend SList;

		public:
			ConstIterator() = default;
			ConstIterator(const Iterator& other);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other"></param>
			/// <returns>if the two iterator point to same nodes</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="other"></param>
			/// <returns>if the two iterator point to different nodes</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// 
			/// </summary>
			/// <returns>the next node of the itereator</returns>
			ConstIterator& operator++();

			/// <summary>
			/// 
			/// </summary>
			/// <returns>the next node of the itereator</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// dereference the Iterator
			/// </summary>
			/// <returns> returns a const T reference </returns>
			const T& operator*() const;

		private:
			ConstIterator(const SList& owner, const Node* node = nullptr);

			const Node* _node{ nullptr };
			const SList* _owner{ nullptr };
		};

		SList() = default;
		SList(std::initializer_list<T> list);

		/// <summary>
		/// Instantiate an list from a existing instance
		/// </summary>
		/// <param name="other">The source instance</param>
		SList(const SList& other);

		/// <summary>
		/// Assign a list
		/// </summary>
		/// <returns>The current instance (i.e. the left-hand side of the assignment)</returns>
		SList& operator=(const SList& other);

		~SList();

		/// <summary>
		/// 
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the list is empty. </exception>
		/// <returns> A reference to the data stored at the front of the list.</returns>
		T& Front();

		/// <summary>
		/// 
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the list is empty. </exception>
		/// <returns>A reference to the data stored at the front of the list.</returns>
		const T& Front() const;

		/// <summary>
		/// 
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the list is empty. </exception>
		/// <returns> A reference to the data stored at the end of the list.</returns>
		T& Back();

		/// <summary>
		/// 
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the list is empty. </exception>
		/// <returns> A reference to the data stored at the end of the list.</returns>
		const T& Back() const;

		/// <summary>
		/// insert given item before the front of the list
		/// </summary>
		/// <param name="value">insert item</param>
		Iterator PushFront(const T& value);

		/// <summary>
		/// remove the first item from the list
		/// </summary>
		void PopFront();

		/// <summary>
		/// append given item after the end of the list.
		/// </summary>
		/// <param name="value">append item</param>
		Iterator PushBack(const T& value);

		/// <summary>
		/// remove the last item from of the list.
		/// </summary>
		void PopBack();

		/// <summary>
		/// 
		/// </summary>
		/// <returns> the size of the list </returns>
		const std::size_t& Size() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns>a bool indicating whether the list contains any items.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// remove all items in the list.
		/// </summary>
		void Clear();

		/// <summary>
		/// Search if the value is inside the SList
		/// </summary>
		/// <param name="value">the searching value</param>
		/// <returns>the Iterator</returns>
		Iterator Find(const T& value);

		/// <summary>
		/// Search if the value is inside the SList
		/// </summary>
		/// <param name="value">the searching value</param>
		/// <returns>the Iterator</returns>
		ConstIterator Find(const T& value) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns>an iterator point to the begin of the SList</returns>
		Iterator begin();

		/// <summary>
		/// 
		/// </summary>
		/// <returns>an iterator point to the begin of a const SList</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns>an iterator point to the begin of a const SList</returns>
		ConstIterator begin() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns>an iterator point to the end of the SList</returns>
		Iterator end();

		/// <summary>
		/// 
		/// </summary>
		/// <returns>an iterator point to the end of a const SList</returns>
		ConstIterator cend() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns>an iterator point to the end of a const SList</returns>
		ConstIterator end() const;
		
		/// <summary>
		/// Insert a value to a desired place in the SList
		/// </summary>
		/// <param name="insertPointer">the place to insert</param>
		/// <param name="value">the value to insert</param>
		Iterator InsertAfter(const Iterator& insertPointer, const T& value);

		/// <summary>
		/// Remove value from the SList 
		/// </summary>
		/// <param name="value">the value to remove</param>
		void Remove(T& value);

		bool Remove(Iterator position);

		template <typename... Args>
		Iterator EmplaceBack(Args&&... args);

		template <typename... Args>
		Iterator EmplaceFront(Args&&... args);

	private:
		std::size_t _size{ 0 };
		Node* _front{ nullptr };
		Node* _back{ nullptr };
	};
}

#include "SList.inl"