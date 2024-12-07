#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <vector>
#include <iostream>

const double alpha = 0.75;
class Node
{
public:
    Node *left;
    Node *right;
    long long value;
    long long total_value;
    long long size;
    bool If_del;
    long long valid_size;
    long long cnt;
    bool Need_rebuild()
    {
        return (double)(left == nullptr ? 0 : left->size) > alpha * (double)size || (double)(right == nullptr ? 0 : right->size) > alpha * (double)size;
    }
    void Update()
    {
        size = !If_del + (left == nullptr ? 0 : left->size) + (right == nullptr ? 0 : right->size);
        valid_size = !If_del + (left == nullptr ? 0 : left->valid_size) + (right == nullptr ? 0 : right->valid_size);
        total_value = !If_del* value + (left == nullptr ? 0 : left->total_value) + (right == nullptr ? 0 : right->total_value);
    }
    Node(long long x) : left(nullptr), right(nullptr), value(x), total_value(x), size(1), If_del(false), valid_size(1) {}
    Node(const Node &other) : left(other.left), right(other.right), value(other.value), total_value(other.total_value),
                              size(other.size), If_del(other.If_del), valid_size(other.valid_size) {}
    ~Node() {}
};

Node *Build(std::vector<Node *> &v, long long l, long long r)
{
    if (l >= r)
    {
        return nullptr;
    }
    long long mid = (l + r) >> 1;
    Node *nd = v[mid];
    nd->left = Build(v, l, mid);
    nd->right = Build(v, mid + 1, r);
    nd->Update();
    return nd;
}

void Dfs(Node *nd, std::vector<Node *> &v)
{
    if (nd == nullptr)
    {
        return;
    }
    Dfs(nd->left, v);
    if (!nd->If_del)
        v.push_back(nd);
    Dfs(nd->right, v);
    if (nd->If_del)
        delete nd;
}

void rebuild(Node *&nd)
{
    std::vector<Node *> v;
    Dfs(nd, v);
    nd = Build(v, 0, v.size());
}

void Insert(long long x, Node *&nd)
{
    if (nd == nullptr)
    {
        nd = new Node(x);
        return;
    }
    nd->size++;
    nd->valid_size++;
    nd->total_value += x;
    if (x >= nd->value)
    {
        Insert(x, nd->right);
    }
    else
    {
        Insert(x, nd->left);
    }
    if (nd->Need_rebuild())
    {
        rebuild(nd);
    }
}
long long Get_rank(Node *nd, long long x)
{
    long long ans = 1;
    while (nd != nullptr)
    {
        if (nd->value >= x)
            nd = nd->left;
        else
        {
            ans += (nd->left == nullptr ? 0 : nd->left->valid_size) + !nd->If_del;
            nd = nd->right;
        }
    }
    return ans;
}
void Delete(Node *&nd, long long x)
{
    if (!nd->If_del && x == nd->value)
    {
        nd->If_del = true;
        --nd->valid_size;
        nd->total_value -= x;
        return;
    }
    --nd->valid_size;
    nd->total_value -= x;
    if (x <= nd->value)
    {
        Delete(nd->left, x);
    }
    else
    {
        Delete(nd->right, x);
    }
}
long long Find(Node *nd, long long k)
{

    while (nd != nullptr)
    {
        if (!nd->If_del && (nd->left == nullptr ? 0 : nd->left->valid_size) + 1 == k)
        {
            return nd->value;
        }
        if ((nd->left == nullptr ? 0 : nd->left->valid_size) >= k)
        {
            nd = nd->left;
        }
        else
        {
            k -= (nd->left == nullptr ? 0 : nd->left->valid_size) + !nd->If_del;
            nd = nd->right;
        }
    }
    return 0;
}
long long Find_sum(Node *nd, long long k)
{
    long long left_v = !nd->If_del * nd->value + (nd->left == nullptr ? 0 : nd->left->total_value);
    long long right_v = (nd->right == nullptr ? 0 : nd->right->total_value);
    long long result = 0;
    while (nd != nullptr)
    {
        if (k * left_v > right_v)
        {
            result = k * left_v - right_v;
            if (nd->left == nullptr)
                break;
            left_v -= !nd->If_del * nd->value;
            right_v += !nd->If_del * nd->value;
            nd = nd->left;
            left_v -= (nd->right == nullptr ? 0 : nd->right->total_value);
            right_v += (nd->right == nullptr ? 0 : nd->right->total_value);
        }
        else if (k * left_v == right_v)
        {
            return 0;
        }
        else
        {
            if (nd->right == nullptr)
                break;
            nd = nd->right;
            left_v += !nd->If_del * nd->value + (nd->left == nullptr ? 0 : nd->left->total_value);
            right_v -= !nd->If_del * nd->value + (nd->left == nullptr ? 0 : nd->left->total_value);
        }
    }
    return result;
}
// you can do data structure initialization in this function
void init(std::size_t n [[maybe_unused]])
{
}

void clear_all(Node *&nd)
{
    if (nd != nullptr)
    {
        clear_all(nd->left);
        clear_all(nd->right);
        delete nd;
    }
}
Node *nd = nullptr;
// You can deconstruct your data structure in this function
void clear()
{
    clear_all(nd);
}

// opt==1 insert x
// opt==2 erase x
// opt==3 query x
// opt==4 query xth
// opt==5 query x*sum(s1...st) - sum(s_t+1...s_m)
long long SetOperation(std::size_t opt, long long x)
{
    switch (opt)
    {
    case 1:
        Insert(x, nd);
        return 0;
        break;

    case 2:
        Delete(nd, x);
        return 0;

    case 3:
        return (x == Find(nd, Get_rank(nd, x)));

    case 4:
        return Find(nd, x);

    case 5:
        return Find_sum(nd, x);
    default:
        return 0;
        break;
    }
}

#endif // CS10123F_PA2_BST