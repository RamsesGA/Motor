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
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using Deque = std::deque<T, A>;

  /**
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using ForwardList = std::forward_list<T, A>;

  /**
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using List = std::list<T, A>;

  /**
  * @brief
  */
  template<typename K,
           typename V,
           typename P = std::less<K>,
           typename A = std::allocator<std::pair<const K, V>>>
    using Map = std::map<K, V, P, A>;

  /**
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using Queue = std::queue<T, std::deque<T, A>>;

  /**
  * @brief
  */
  template<typename T,
           typename P = std::less<T>,
           typename A = std::allocator<T>>
    using Set = std::set<T, P, A>;

  /**
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using Stack = std::stack<T, std::deque<T, A>>;

  /**
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using Vector = std::vector<T, A>;

  /**
  * @brief basic_string<char, char_traits<char>, allocator<char>>;
  */
  using String = std::string;

  using WString = std::wstring;

  using IfStream = std::ifstream;

  using FStream = std::fstream;

  using OfStream = std::ofstream;

  using StrStream = std::stringstream;
}