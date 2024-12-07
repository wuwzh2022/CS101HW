#ifndef CS10123F_PA1_HASHTABLE
#define CS10123F_PA1_HASHTABLE

#include <cstdint>
#include <cstdio>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "hash_types.hpp"

template <> struct std::hash<Website> {
  static const uint32_t base = 1009u; // 1009 is a prime number.
  uint32_t operator()(const Website &S) const noexcept {
    // ------------------------START YOUR CODE----------------
    int length = S.name.length();
    uint32_t result = 0;
    uint32_t b = 1;
    for (int i = length - 1; i >= 0; i--)
    {
      result += S.name[i] * b;
      b *= base;
    }
    return result;
    // ------------------------END OF YOUR CODE----------------
  }
};

template <class Key = Website, class T = IPv4> // template for key type class
                                               // Key and value type class T
class HashTable {
private:
  enum entrystate { Unused, Used, Erased };
  const size_t Mod = 1000037u; // 1000037 is a prime number
  std::vector<T> Table;        // main table stores value
  std::vector<Key> Stored_Key; // used for storing original key
  std::vector<entrystate>
      State; // keeps the state of every table entry : {Unused, Used or Erased};
  std::vector<size_t> Used_pos; // hint: used for function clear()

public:
  static const std::size_t npos = -1;
  HashTable() : Table(Mod), Stored_Key(Mod), State(Mod, entrystate::Unused) {}

  const T &operator[](const Key &x) const {
    std::size_t pos = find(x);
    if (pos == npos)
      throw std::invalid_argument("Key does not exist");
    return Table[pos];
  }

  // ------------------------START YOUR CODE----------------

  std::size_t Search(const Key &x) const {
    std::size_t Start = std::hash<Key>{}(x) % Mod;
    std::size_t e_pos = npos;
    while (State[Start] != entrystate::Unused)
    {
      if(State[Start] == entrystate::Erased){
        e_pos = Start;
        Start++;
        continue;
      }
      if(Stored_Key[Start] == x)
        return Start;
      Start++;
    }
    if(e_pos != npos) return e_pos;
    return Start;
  }

  std::size_t find(const Key &x) const {
    std::size_t Start = std::hash<Key>{}(x) % Mod;
    while (State[Start] != entrystate::Unused)
    {
      if(State[Start] == entrystate::Erased){
        Start++;
        continue;
      }
      if(Stored_Key[Start] == x)
        return Start;
      Start++;
    }
    return npos;
  }

  void insert(const Key &x, const T& value) {
    auto Start = Search(x);
    Stored_Key[Start] = x;
    Table[Start] = value;
    Used_pos.push_back(Start);
    State[Start] = entrystate::Used;
    return;
  }

  bool erase(const Key &x) {
    if(find(x) == npos){
      return false;
    }
    else{
      State[find(x)] = entrystate::Erased;
      return true;
    }
  }

  void clear() {
    for(auto pos:Used_pos){
      State[pos] = entrystate::Unused;
    }
    Used_pos.clear();
  }
};

#endif // CS10123F_PA1_HASHTABLE