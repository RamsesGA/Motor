#pragma once

/*****************************************************************************/
/**
* Type objects
*/
/*****************************************************************************/
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

/*****************************************************************************/
/**
* Memory
*/
/*****************************************************************************/
#include <memory>

/*****************************************************************************/
/**
* Containers
*/
/*****************************************************************************/
#include <map>
#include <set>
#include <list>
#include <stack>
#include <deque>
#include <queue>
#include <array>
#include <vector>
#include <string>
#include <bitset>
#include <forward_list>

/*****************************************************************************/
/**
* Reading documents and writing documents
*/
/*****************************************************************************/
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

/*****************************************************************************/
/**
* Algorithms and functions
*/
/*****************************************************************************/
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>

namespace gaEngineSDK {
  /**
  * @brief Double-ended queues are sequence containers with dynamic sizes that
  *        can be expanded or contracted on both ends (either its front or its back).
  */
  template<typename T, typename A = std::allocator<T>>
  using Deque = std::deque<T, A>;

  /**
  * @brief Sequence containers that allow constant time insert
  *        and erase operations anywhere within the sequence.
  */
  template<typename T, typename A = std::allocator<T>>
  using ForwardList = std::forward_list<T, A>;

  /**
  * @brief Sequence containers that allow constant time insert and erase operations 
  *        anywhere within the sequence, and iteration in both directions.
  */
  template<typename T, typename A = std::allocator<T>>
  using List = std::list<T, A>;

  /**
  * @brief Associative containers that store elements 
  *        formed by a combination of a key value and a mapped value,
  *        following a specific order.
  */
  template<typename K,
           typename V,
           typename P = std::less<K>,
           typename A = std::allocator<std::pair<const K, V>>>
  using Map = std::map<K, V, P, A>;

  /**
  * @brief Type of container adaptor, specifically designed to operate in a FIFO context.
  */
  template<typename T, typename A = std::allocator<T>>
  using Queue = std::queue<T, std::deque<T, A>>;

  /**
  * @brief Containers that store unique elements following a specific order.
  */
  template<typename T,
           typename P = std::less<T>,
           typename A = std::allocator<T>>
  using Set = std::set<T, P, A>;

  /**
  * @brief Type of container adaptor, specifically designed to operate in a LIFO context.
  */
  template<typename T, typename A = std::allocator<T>>
  using Stack = std::stack<T, std::deque<T, A>>;

  /**
  * @brief Normal sequence containers representing arrays.
  */
  template<typename T, typename A = std::allocator<T>>
  using Vector = std::vector<T, A>;

  /**
  * @brief Unique Pointer.
  */
  template<typename T>
  using UPtr = std::unique_ptr<T>;

  /**
  * @brief Shared pointer.
  */
  template<typename T>
  using SPtr = std::shared_ptr<T>;

  /**
  * @brief Weak shared pointer.
  */
  template<typename T>
  using WeakSPtr = std::weak_ptr<T>;

  /**
  * @brief Objects that represent sequences of characters.
  */
  using String = std::string;

  /**
  * @brief String class for wide characters.
  */
  using WString = std::wstring;

  /**
  * @brief Input file stream class.
  */
  using IfStream = std::ifstream;

  /**
  * @brief Input/output file stream class.
  */
  using FStream = std::fstream;

  /**
  * @brief Output file stream.
  */
  using OfStream = std::ofstream;

  /**
  * @brief Input/output string stream.
  */
  using StrStream = std::stringstream;

  class StringID
  {
   public:
    StringID() = default;

    StringID(String data) : m_data(data) { }

    ~StringID() = default;

    uint32 m_id = 0;

    String m_data;
  };
}