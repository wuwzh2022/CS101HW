#include "graph.hpp"
#include<iostream>

auto Graph::dijkstra(VertexID source) const -> std::vector<Weight>
{
  std::vector<Weight> dist(numVertices(), infinity);
  std::vector<bool> visited(numVertices(), false);
  dist[source] = 0;
  auto cmp = [&dist](VertexID x, VertexID y)
  {
    return dist[x] > dist[y];
  };
  std::priority_queue<std::size_t, std::vector<std::size_t>, decltype(cmp)>
      minHeap(cmp);
  // FIXME: This is a naive O(V^2) implementation. Overwrite it to make it run
  // within O(ElogV) time, which is more efficient when E = o(V^2/logV).
  auto length = numVertices();
  for (auto i = 0; i <length; ++i)
  {
    minHeap.push(i);
  }

  for (auto rest = numVertices() ; rest > 0; --rest)
  {
    auto current = minHeap.top();
    while(visited[current]){
      minHeap.pop();
      current = minHeap.top();
    }
    minHeap.pop();
    visited[current] = true;
    for (auto [to, weight] : mAdjacent[current])
    {
      if (dist[current] + weight < dist[to])
      {
        dist[to] = dist[current] + weight;
        minHeap.push(current);
      }
    }
    
  }
  return dist;
}

auto Graph::bellmanFord(VertexID source) const
    -> std::optional<std::vector<Weight>> {
  // TODO: Implement this.
}