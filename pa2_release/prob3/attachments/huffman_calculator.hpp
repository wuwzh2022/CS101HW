#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <tuple>
#include <vector>
#include <algorithm>

void swap(std::pair<size_t, size_t> &a, std::pair<size_t, size_t> &b)
{
  auto x = a;
  a = b;
  b = x;
}

void insert(std::vector<std::pair<size_t, size_t>> &heap, std::pair<size_t, size_t> new_pair)
{
  int x = heap.size();
  heap.push_back(new_pair);
  while (x != 1 && heap[x].first < heap[x / 2].first)
  {
    swap(heap[x], heap[x / 2]);
    x = x / 2;
  }
}

void pop(std::vector<std::pair<size_t, size_t>> &heap)
{
  int length = heap.size() - 1;
  int x = 1;
  swap(heap[x], heap[length]);
  heap.pop_back();
  while (2 * x < length - 1 && (heap[x] > heap[2 * x] || heap[x] > heap[2 * x + 1]))
  {
    if (heap[2 * x] > heap[2 * x + 1])
    {
      swap(heap[x], heap[2 * x + 1]);
      x = 2 * x + 1;
    }
    else
    {
      swap(heap[x], heap[2 * x]);
      x = 2 * x;
    }
  }
  if (2 * x == length - 1)
  {
    if (heap[x] > heap[2 * x])
    {
      swap(heap[x], heap[2 * x]);
    }
  }
}

size_t get_huffman_length(const std::vector<std::pair<size_t, size_t>> &data)
{
  // TODO: Implement this.
  std::vector<std::pair<size_t, size_t>> heap(data);
  heap.insert(heap.begin(), std::pair<size_t, size_t>(0, 0));
  std::sort(heap.begin(), heap.end());
  size_t total_length = 0;
  while (heap.size() != 2 || heap[1].second != 1)
  {
    if (heap[1].second % 2 == 0)
    {
      total_length += heap[1].first * heap[1].second;
      insert(heap, std::pair<size_t, size_t>(heap[1].first * 2, heap[1].second / 2));
      pop(heap);
    }
    else
    {
      if (heap[1].second != 1)
      {
        total_length += heap[1].first * (heap[1].second - 1);
        insert(heap, std::pair<size_t, size_t>(heap[1].first * 2, (heap[1].second - 1) / 2));
        heap[1].second = 1;
      }
      if (heap[1].second == 1)
      {
        auto value = heap[1].first;
        pop(heap);
        if (heap[1].second != 1)
        {
          heap[1].second--;
          total_length += heap[1].first + value;
          insert(heap, std::pair<size_t, size_t>((heap[1].first + value), 1));
        }
        else
        {
          total_length += heap[1].first + value;
          insert(heap, std::pair<size_t, size_t>((heap[1].first + value), 1));
          pop(heap);
        }
      }
    }
  }
  return total_length;
}
#endif // HUFFMAN_CALCULATOR_HPP