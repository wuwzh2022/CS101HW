#ifndef CS101_SLIST_HPP
#define CS101_SLIST_HPP

#include <algorithm>
#include <concepts>
#include <iterator>
#include <memory>

#include "slist_node.hpp"

template <typename T, bool IsConst>
struct SlistIterator
{
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using reference = std::conditional_t<IsConst, const T &, T &>;
  using pointer = std::conditional_t<IsConst, const T *, T *>;
  using iterator_category = std::forward_iterator_tag;

private:
  using node_type = SlistNode<T>;
  using self_type = SlistIterator<T, IsConst>;

  node_type *m_node{};

  template <typename OtherDecayed>
  static constexpr auto isAddingLowLevelConst =
      IsConst && std::is_same_v<OtherDecayed, SlistIterator<T, false>>;

public:
  SlistIterator() = default;

  explicit SlistIterator(const std::unique_ptr<node_type> &up)
      : m_node{up.get()} {}

  template <typename Other>
    requires isAddingLowLevelConst<std::decay_t<Other>>
  SlistIterator(Other &&other) : m_node{other.base()}
  {
  }

  auto base() const { return m_node; }

  // TODO: IMPLEMENT THIS.
  reference operator*() const
  {
    return base()->value;
  }

  pointer operator->() const { return std::addressof(operator*()); }

  // TODO: IMPLEMENT THIS.
  self_type &operator++()
  {
    m_node = m_node->next.get();
    return *this;
  }

  self_type operator++(int)
  {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  // TODO: IMPLEMENT THIS.
  template <bool C>
  bool operator==(const SlistIterator<T, C> &other) const
  {
    return base() == other.base();
  }
};

template <typename T>
class Slist
{
public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = SlistIterator<T, false>;
  using const_iterator = SlistIterator<T, true>;

private:
  using node_type = SlistNode<T>;
  using node_uptr = std::unique_ptr<node_type>;

  node_uptr m_head;

public:
  Slist() = default;

  // TODO: IMPLEMENT THIS.
  Slist(const Slist &other)
  {
    if (other.empty())
      return;
    m_head = std::make_unique<node_type>(std::move(m_head), other.m_head->value);
    auto node = begin().base();
    auto ptr = other.begin();
    ptr++;
    while (ptr != other.end())
    {
      node->next = std::make_unique<node_type>(std::move(node->next), *ptr);
      node = (node->next).get();
      ptr++;
    }
  }

  void swap(Slist &other) noexcept { m_head.swap(other.m_head); }

  Slist &operator=(const Slist &other)
  {
    Slist(other).swap(*this);
    return *this;
  }

  Slist(Slist &&) noexcept = default;

  Slist &operator=(Slist &&) noexcept = default;

  ~Slist() = default;

  // TODO: IMPLEMENT THIS.
  Slist(size_type count, const T &value)
  {
    if (count == 0)
      return;
    m_head = std::make_unique<node_type>(std::move(m_head), value);
    auto node = begin().base();
    for (size_type i = 0; i < count - 1; i++)
    {
      node->next = std::make_unique<node_type>(std::move(node->next), value);
      node = (node->next).get();
    }
  }

  // TODO: IMPLEMENT THIS.
  explicit Slist(size_type count)
  {
    if (count == 0)
      return;
    m_head = std::make_unique<node_type>(std::move(m_head));
    auto node = begin().base();
    for (size_type i = 0; i < count - 1; i++)
    {
      node->next = std::make_unique<node_type>(std::move(node->next));
      node = (node->next).get();
    }
  }

  // TODO: IMPLEMENT THIS.
  Slist(std::input_iterator auto b, std::input_iterator auto e)
  {
    if(b == e )
      return;
    m_head = std::make_unique<node_type>(std::move(m_head), *b);
    auto node = begin().base();
    auto ptr = b;
    ptr++;
    while (ptr != e)
    {
      node->next = std::make_unique<node_type>(std::move(node->next), *ptr);
      node = (node->next).get();
      ptr++;
    }
  }

  Slist(std::initializer_list<T> init) : Slist(init.begin(), init.end()) {}

  iterator begin() { return iterator(m_head); }
  iterator end() { return {}; }
  const_iterator cbegin() const { return const_iterator(m_head); }
  const_iterator cend() const { return {}; }
  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }

  void push_front(const T &x)
  {
    emplace_front(x); // Why?
  }

  void push_front(T &&x)
  {
    emplace_front(std::move(x)); // Why?
  }

  // TODO: IMPLEMENT THIS.
  template <typename... Types>
  reference emplace_front(Types &&...args)
  {
    m_head = std::make_unique<node_type>(std::move(m_head), std::forward<Types>(args)...);
    return *(begin());
  }

  // TODO: IMPLEMENT THIS.
  void pop_front()
  {
    if(empty())
      return;
    m_head = std::move(m_head->next);
  }

  void clear()
  {
    m_head.reset(); // Why?
  }

  // TODO: IMPLEMENT THIS.
  void insert_after(const_iterator pos, const T &x)
  {
    emplace_after(pos, x);
  }

  // TODO: IMPLEMENT THIS.
  void insert_after(const_iterator pos, T &&x)
  {
    emplace_after(pos, std::move(x));
  }

  // TODO: IMPLEMENT THIS.
  template <typename... Types>
  iterator emplace_after(const_iterator pos, Types &&...args)
  {
    pos.base()->next = std::make_unique<node_type>(std::move(pos.base()->next), std::forward<Types>(args)...);
    return iterator(pos.base()->next);
  }

  // TODO: IMPLEMENT THIS.
  iterator erase_after(const_iterator pos)
  {
    pos.base()->next = std::move(pos.base()->next->next);
    return iterator(pos.base()->next);
  }

  // TODO: IMPLEMENT THIS.
  iterator erase_after(const_iterator pos, const_iterator last)
  {
    auto itr = pos;
    int cnt = 0;
    while (itr != last)
    {
      itr++;
      cnt++;
    }
    cnt--;
    for (int i = 0; i < cnt; i++)
    {
      erase_after(pos);
    }
    return iterator(pos.base()->next);
  }

  // TODO: IMPLEMENT THIS.
  bool empty() const
  {
    return begin() == end();
  }

  // TODO: IMPLEMENT THIS.
  size_type size() const
  {
    size_type cnt = 0;
    auto itr = begin();
    while (itr != end())
    {
      itr++;
      cnt++;
    }
    return cnt;
  }

  reference front() { return m_head->value; }
  const_reference front() const { return m_head->value; }

  bool operator==(const Slist<T> &other) const
  {
    return std::ranges::equal(*this, other);
  }

  auto operator<=>(const Slist<T> &other) const
  {
    return std::lexicographical_compare_three_way(begin(), end(), other.begin(),
                                                  other.end());
  }

  void merge(Slist<T> &other) { merge(std::move(other)); }

  void merge(Slist<T> &&other) { merge(std::move(other), std::less<>{}); }

  void merge(Slist<T> &other, auto compare)
  {
    merge(std::move(other), compare);
  }

  // TODO: IMPLEMENT THIS.
  void merge(Slist<T> &&other, auto compare)
  {
    if (empty())
      m_head.swap(other.m_head);
    if (other.empty())
      return;
    if (begin().base() == other.begin().base())
      return;
    if(compare(other.m_head->value, m_head->value))
    {
      m_head.swap(other.m_head);
      other.m_head.swap(m_head->next);
    }
    auto itr = begin();
    while (itr.base()->next != nullptr && !other.empty())
    {
      if (compare(other.m_head->value, itr.base()->next->value))
      {
        itr.base()->next.swap(other.m_head);
        other.m_head.swap(itr.base()->next->next);
      }
      itr++;
    }
    if (!other.empty())
    {
      itr.base()->next.swap(other.m_head);
    }
  }

  void sort() { sort(std::less<>{}); }

  // TODO: IMPLEMENT THIS.
  void sort(auto compare)
  {
    if (size() <= 1)
    {
      return;
    }
    auto itr = begin();
    auto length = size();
    for (size_type i = 0; i < (length - 2) / 2; i++)
    {
      itr++;
    }
    auto list2 = Slist();
    auto list1 = Slist();
    list2.m_head.swap(itr.base()->next);
    list1.m_head.swap(m_head);
    list1.sort(compare);
    list2.sort(compare);
    list1.merge(list2, compare);
    m_head.swap(list1.m_head);
  }
};

template <std::input_iterator Iterator>
Slist(Iterator, Iterator)
    -> Slist<typename std::iterator_traits<Iterator>::value_type>;

namespace std
{
  template <typename T>
  void swap(Slist<T> &a, Slist<T> &b) noexcept
  {
    a.swap(b);
  }
} // namespace std

#endif // CS101_SLIST_HPP