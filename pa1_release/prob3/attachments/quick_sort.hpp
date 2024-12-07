#include <iostream>
#include <vector>

auto random_choice(auto begin, auto end)
{
  // This is only a possible implementation of random_choice.
  // You can use this for your convenience or design a better one.
  std::size_t length = end - begin;
  std::size_t idx = std::rand() % length;
  return begin + idx;
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end, auto compare)
{
  // TODO: IMPLEMENT THIS.
  if (end - begin <= 1)
    return;
  auto pivot = random_choice(begin, end);
  auto high = end - 2;
  std::swap(*pivot, *(end - 1));
  auto low = begin;
  while (compare(*(end - 1), *high) && high > begin)
    high--;
  while (compare(*low, *(end - 1)) &&  low < end - 1)
    low++;
  while (low < high)
  {
    std::swap(*low, *high);
    high--;
    low++;
    while (compare(*(end - 1), *high) && high > begin)
      high--;
    while (compare(*low, *(end - 1)) && low < end - 1)
      low++;
  }
  std::swap(*low, *(end - 1));
  quick_sort(begin, low, compare);
  quick_sort(low + 1, end, compare);
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end)
{
  quick_sort(begin, end, std::less<>{});
}
