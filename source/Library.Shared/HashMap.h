#pragma once

#include "Vector.h"
#include "SList.h"
#include "DefaultHash.h"

namespace FieaGameEngine
{
	template <typename TKey, typename TData>
	class HashMap final
	{
		public:
			using PairType = std::pair<const TKey, TData>;
			using ChainType = SList<PairType>;
			using BucketType = Vector<ChainType>;
			using HashFunctor = std::function<std::size_t(TKey)>;

			class Iterator
			{
				friend HashMap;
			public:
				Iterator() = default;
				Iterator(const Iterator & other) = default;
				Iterator(Iterator && other) = default;
				Iterator& operator=(Iterator && other) = default;
				~Iterator() = default;

				/// <summary>
				/// 
				/// </summary>
				/// <param name="other">comparision Iterator</param>
				/// <returns>if the iterator is the same</returns>
				bool operator==(const Iterator & other) const;

				/// <summary>
				/// 
				/// </summary>
				/// <param name="other">comparision Iterator</param>
				/// <returns>if the iterator is not the same</returns>
				bool operator!=(const Iterator & other) const;

				/// <summary>
				/// 
				/// </summary>
				/// <param name="other">the Iterator intend to copy</param>
				/// <returns>the Iterator you copy</returns>
				Iterator& operator= (const Iterator & other);

				/// <summary>
				/// Prefix increment
				/// </summary>
				/// <returns>the next index of the itereator</returns>
				Iterator& operator++();

				/// <summary>
				/// Pistfix increment
				/// </summary>
				/// <returns>the next index of the itereator</returns>
				Iterator operator++(int);

				/// <summary>
				/// Dereference
				/// </summary>
				/// <returns>Get the Pair of the Iterator current Node</returns>
				PairType& operator*() const;

			private:
				Iterator(HashMap & owner, size_t index, typename ChainType::Iterator chainIterator);

				HashMap* _owner{ nullptr };
				size_t _index{ 0 };
				typename ChainType::Iterator _chainIterator;
			};

			class ConstIterator
			{
				friend HashMap;

			public:
				ConstIterator() = default;
				ConstIterator(const ConstIterator & other) = default;
				ConstIterator(ConstIterator && other) = default;
				ConstIterator& operator=(ConstIterator && other) = default;
				~ConstIterator() = default;


				/// <summary>
				/// 
				/// </summary>
				/// <param name="other">comparision const Iterator</param>
				/// <returns>if the const iterator is the same</returns>
				bool operator==(const ConstIterator & other) const;

				/// <summary>
				/// 
				/// </summary>
				/// <param name="other">comparision const Iterator</param>
				/// <returns>if the const iterator is not the same</returns>
				bool operator!=(const ConstIterator & other) const;

				/// <summary>
				/// 
				/// </summary>
				/// <param name="other">the Iterator intend to copy</param>
				/// <returns>the Iterator you copy</returns>
				ConstIterator& operator=(const Iterator & other);

				/// <summary>
				/// 
				/// </summary>
				/// <param name="other">the Const Iterator intend to copy</param>
				/// <returns>the const Iterator you copy</returns>
				ConstIterator& operator=(const ConstIterator& other);

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

				/// <summary>
				/// Dereference
				/// </summary>
				/// <returns>Get the Pair of the Const Iterator current Node</returns>
				const PairType& operator*() const;

			private:
				ConstIterator(const HashMap& owner, size_t index, typename ChainType::ConstIterator chainIterator);

				const HashMap* _owner{ nullptr };
				size_t _index{ 0 };
				typename ChainType::ConstIterator _chainIterator;
			};

			HashMap(const size_t size = 11, const HashFunctor hashFunctor = DefaultHash<TKey>{});
			~HashMap() = default;

			HashMap(HashMap&& otehr) = default;
			HashMap& operator=(HashMap&& other) = default;

			HashMap(std::initializer_list<PairType> list, const HashFunctor _hashFunctor = DefaultHash<TKey>{});

			/// <summary>
			/// Instantiate a HashMap from an existing instance
			/// </summary>
			/// <typeparam name="TKey">Key</typeparam>
			/// <typeparam name="TData">Data</typeparam>
			HashMap(const HashMap& other) = default;

			/// <summary>
			/// Assign a Hashmap
			/// </summary>
			/// <typeparam name="TKey">Key</typeparam>
			/// <typeparam name="TData">Data</typeparam>
			HashMap& operator=(const HashMap& other) = default;

			/// <summary>
			/// Find if a Key is alreay in Hashmap
			/// </summary>
			/// <typeparam name="TKey">the key intend to find</typeparam>
			/// <typeparam name="TData"></typeparam>
			Iterator Find(const TKey& key);

			/// <summary>
			/// Find if a Key is alreay in Hashmap
			/// </summary>
			/// <typeparam name="TKey">the key intend to find</typeparam>
			/// <typeparam name="TData"></typeparam>
			ConstIterator Find(const TKey& key) const;

			Iterator Find(const TKey& key, std::size_t& index);
			ConstIterator Find(const TKey& key, std::size_t& index) const;

			/// <summary>
			/// Insert a pair of value into the Hashmap
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			std::pair<Iterator, bool> Insert(const PairType & pair);

			/// <summary>
			/// Index Operator, takes a “key” argument of the appropriate type and which returns a reference to the TData part
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			TData& operator[](const TKey& key);

			/// <summary>
			/// takes a “key” argument (as above) and which returns nothing
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			void Remove(const TKey& key);
			
			/// <summary>
			/// Clear the hashmap
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			void Clear();

			/// <summary>
			/// return the number of element the user insert
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			const std::size_t& Size() const;

			/// <summary>
			/// check if the hashmap contains certain Key
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			const bool ContainsKey(const TKey& key);

			/// <summary>
			/// check if the hashmap contains certain Key
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			const bool ContainsKey(const TKey& key) const;

			/// <summary>
			/// Get the Date in the certain Key
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			TData& At(const TKey& key);

			/// <summary>
			/// Get the Date in the certain Key
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			const TData& At(const TKey& key) const;

			/// <summary>
			/// begin of the iterator
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			Iterator begin();

			/// <summary>
			/// begin of the iterator
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			ConstIterator cbegin() const;

			/// <summary>
			/// begin of the iterator
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			ConstIterator begin() const;

			/// <summary>
			/// the end of the iterator
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			Iterator end();

			/// <summary>
			/// the end of the iterator
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			ConstIterator cend() const;

			/// <summary>
			/// the end of the iterator
			/// </summary>
			/// <typeparam name="TKey"></typeparam>
			/// <typeparam name="TData"></typeparam>
			ConstIterator end() const;

			template <typename... Args>
			std::pair<Iterator, bool> Emplace(Args&&... args);

			//std::pair<Iterator, bool> Insert(const PairType& item);
			//std::pair<Iterator, bool> Insert(PairType&& item);

			HashMap& operator=(std::initializer_list<PairType> list);

		private:
			std::size_t _size{ 0 };
			BucketType _bucket;
			HashFunctor _hashFunctor;
	};
}
#include "HashMap.inl"