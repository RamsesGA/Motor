#pragma once

/*****************************************************************************/
/**
* Type objects
*/
/*****************************************************************************/
#include <cassert>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <ctime>

#include <memory>

/*****************************************************************************/
/**
* Containers
*/
/*****************************************************************************/

#include <vector>
#include <stack>
#include <map>
#include <string>
#include <set>
#include <list>
#include <forward_list>
#include <deque>
#include <queue>
#include <bitset>
#include <array>

/*****************************************************************************/
/**
* reading documents and writing documents
*/
/*****************************************************************************/

#include <fstream>
#include <iostream>
#include <sstream>

namespace gaEngineSDK {

  /**
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using Vector = std::vector<T, A>;

  /**
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using Stack = std::stack<T, std::deque<T, A>>;

  /**
  * @brief
  */
  template<typename K, 
           typename V, 
           typename P = std::less<K>,
           typename A = std::allocator<std::pair<const K, V>>>
  using Map = std::map<K, V, P, A>;


  /**
  * @brief basic_string<char, char_traits<char>, allocator<char>>;
  */
  /*template<typename C = char, 
           typename CT = std::char_traits<C>, 
           typename A = std::allocator<C>>
  using String = std::string<C, CT, A>;*/

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
  using List = std::list<T, A>;

  /**
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using ForwardList = std::forward_list<T, A>;

  /**
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using Deque = std::deque<T, A>;

  /**
  * @brief
  */
  template<typename T, typename A = std::allocator<T>>
  using Queue = std::queue<T, std::deque<T, A>>;
}