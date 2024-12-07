#include <vector>
#include <iostream>
#include <cmath>
#include <numbers>
#include <algorithm>
using namespace std;
bool compare(std::pair<std::pair<size_t, size_t>, size_t> a, std::pair<std::pair<size_t, size_t>, size_t> b)
{
    return a.second > b.second;
}
size_t find_i(size_t i, std::vector<size_t> &parent)
{
    if(parent[i]==i){
        return i;
    }
    else{
        parent[i] = find_i(parent[i],parent);
        return parent[i];
    }
}

void set_one_union(size_t i, size_t j, std::vector<size_t> &parent, std::vector<size_t> &height, size_t &sum, size_t w)
{
    i = find_i(i, parent);
    j = find_i(j, parent);
    if (i != j)
    {
        if (height[i] > height[j])
        {
            parent[j] = i;
        }
        else
        {
            if (height[i] == height[j])
            {
                height[j]++;
            }
            parent[i] = j;
        }
        sum += w;
    }
}

int main()
{
    size_t m;
    size_t n;
    size_t l, r, w;
    size_t sum = 0;
    cin >> m >> n;
    n += 1;
    vector<pair<pair<size_t, size_t>, size_t>> graph(m);
    for (size_t i = 0; i < m; ++i)
    {
        cin >> l >> r >> w;
        graph[i] = make_pair(make_pair(l-1, r), w);
    }
    std::sort(graph.begin(), graph.end(), compare);
    std::vector<size_t> parent(n);
    for (size_t i = 0; i < n; i++)
    {
        parent[i] = i;
    }
    std::vector<size_t> height(n, 1);
    for (size_t i = 0; i < m; i++)
    {
        set_one_union(graph[i].first.first, graph[i].first.second, parent, height, sum, graph[i].second);
    }
    cout << sum << endl;
    return 0;
}