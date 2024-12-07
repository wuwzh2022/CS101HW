#include <iostream>
#include <vector>
#include <numbers>
#include <cmath>
using namespace std;

size_t inverse[6002][6002] = {0};
size_t normal[6002][6002] = {0};
bool If_buy[6002][6002] = {false};
size_t v[6002] = {0};
size_t c[6002] = {0};

size_t without(size_t k, size_t m, size_t &max)
{
    max = 0;
    for (size_t i = 0; i <= m; i++)
    {
        if (normal[k - 1][i] + inverse[k + 1][m - i] > max)
        {
            max = normal[k - 1][i] + inverse[k + 1][m - i];
        }
    }
    return max;
}

int main()
{
    size_t m, n;
    cin >> n >> m;
    for (size_t i = 1; i < n + 1; i++)
    {
        cin >> v[i] >> c[i];
    }
    // inverse
    for (size_t i = n; i > 0; i--)
    {
        for (size_t j = 1; j < m + 1; j++)
        {
            if (c[i] > j)
            {
                inverse[i][j] = inverse[i + 1][j];
            }
            else
            {
                if (inverse[i + 1][j] > inverse[i + 1][j - c[i]] + v[i])
                {
                    inverse[i][j] = inverse[i + 1][j];
                }
                else
                {
                    inverse[i][j] = inverse[i + 1][j - c[i]] + v[i];
                    If_buy[i][j] = true;
                }
            }
        }
    }
    // normal
    for (size_t i = 1; i < n + 1; i++)
    {
        for (size_t j = 1; j < m + 1; j++)
        {
            if (c[j] > m)
            {
                normal[i][j] = normal[i - 1][j];
            }
            else
            {
                if (normal[i - 1][j] > normal[i - 1][j - c[i]] + v[i])
                {
                    normal[i][j] = normal[i - 1][j];
                }
                else
                {
                    normal[i][j] = normal[i - 1][j - c[i]] + v[i];
                }
            }
        }
    }
    // cout the result
    size_t i = 1;
    size_t j = m;
    size_t max = 0;
    vector<int> res;
    while (i < n + 1 && j > 0)
    {
        if (If_buy[i][j]) 
        {
            cout << i << " ";
            res.push_back(i);
            j -= c[i];
        }
        i++;
    }
    cout << endl;
    auto length = res.size();
    for (size_t k = 0; k < length; k++)
    {
        cout << v[res[k]] - inverse[1][m] + without(res[k], m, max) << " ";
    }
    cout << endl;
}