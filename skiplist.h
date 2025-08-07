#ifndef SKIPLIST_WU_MANBER_H
#define SKIPLIST_WU_MANBER_H

#include <vector>
#include <string>
#include <functional>
#include <cstdlib>
#include <iostream>
#include <cmath>    // 提供log2()和pow()函数
#include <cstring>  // 提供memset()函数

using namespace std;

#define SHIFT_INDEX 256*256

size_t hashPrefixKey(const string& key,int min_pat_len) // 前缀表哈希函数
{
  unsigned int h = 0;
  for(int i = 0; i < min_pat_len; i++)
  {
    h = h * 31 + (unsigned char)key[i];
  }
  return h % SHIFT_INDEX;
}

template <typename V>
class Node
{
public:
    Node(size_t hashKey, const V& v, int level) : hash_key(hashKey), value(v), node_level(level)
    {
        this->forward = new Node<V>* [level + 1];
        memset(this->forward, 0, sizeof(Node<V>*) * (level + 1));
    }
    ~Node()
    {
        delete[] forward;
    }

    Node<V>* get_next(int level) const { return forward[level]; }
    void set_next(int level, Node<V>* next) { forward[level] = next; }

    size_t hash_key;
    V value;
    int node_level;

private:
    Node<V> **forward;
};

class WuManberSkipList
{
public:
    WuManberSkipList(int max_level = 16, size_t prefix_len = 3);
    ~WuManberSkipList();

    void insert_pattern(const string& pattern);
    bool search_exact_match(const string& text);
    void optimize_for_wumanber();
    void clear();

private:
    Node<string>** find_update_nodes(size_t hash_key, bool for_delete = false);
    int get_random_level();
    void insert_node(size_t hash_key, const string& pattern);

private:
    Node<string>* header;
    int max_level;
    int current_level;
    size_t prefix_length;
    size_t pattern_count;
    double probability;
};

// 初始化跳表
WuManberSkipList::WuManberSkipList(int maxLevel, size_t prefix_len) : max_level(maxLevel), 
                                    current_level(0), prefix_length(prefix_len), pattern_count(0), probability(0.5)
{
    header = new Node<string>(0, "", max_level);
}

WuManberSkipList::~WuManberSkipList()
{
    clear();
    delete header;
}

// 递归清空跳表
void WuManberSkipList::clear()
{
    Node<string>* current = header->get_next(0);

    while(current)
    {
        Node<string>* next = current->get_next(0);
        delete current;
        current = next;
    }
    for(int i = 0; i <= current_level; i++)
    {
        header->set_next(i, nullptr);
    }
    current_level = 0;
    pattern_count = 0;
}

// 为Wu-Manber算法优化跳表, 当模式数量变多时，有更高的概率出现更高层
void WuManberSkipList::optimize_for_wumanber()
{
    // 优化策略：
    // 1. 调整层级分布以适应前缀匹配
    // 2. 平衡跳表高度与哈希冲突率

    // 计算最优概率 (log2(N)/B，其中B是前缀长度)
    if (pattern_count > 100)
    {
        probability = 1.0 - (1.0 / pow(2, log2(pattern_count) / prefix_length));
    }
}

// 插入模式串
void WuManberSkipList::insert_pattern(const string& pattern)
{
    if(pattern.size() < prefix_length) return;

    size_t hash_key = hashPrefixKey(pattern, pattern.size());
    Node<string>** update = find_update_nodes(hash_key);
    Node<string>* current = update[0]->get_next(0);

    insert_node(hash_key, pattern); // 创建新节点

    delete[] update;
    pattern_count++;
}

// 添加新层级节点
void WuManberSkipList::insert_node(size_t hash_key, const string& pattern)
{
    int new_level = get_random_level();
    if(new_level > current_level)
    {
        for(int i = current_level + 1; i <= new_level; i++)
        {
            header->set_next(i, nullptr);
        }
        current_level = new_level;
    }

    Node<string>* new_node = new Node<string>(hash_key, pattern, new_level); // 创建新节点

    Node<string>** update = find_update_nodes(hash_key); // 调整指针
    for(int i = 0; i <= new_level; i++)
    {
        new_node->set_next(i, update[i]->get_next(i));
        update[i]->set_next(i, new_node);
    }

    delete[] update;
}

// 查找完整的模式的匹配（默认不会有重复的模式串，但是会有相同的哈希值的模式串）
bool WuManberSkipList::search_exact_match(const string& text)
{
    if(text.length() < prefix_length) return false;

    size_t hash_key = hashPrefixKey(text, text.length()); // 查找具体模式
    Node<string>* current = header;
    bool found = false;

    // 在跳表中查找匹配
    for(int i = current_level; i >= 0; i--)
    {
        while(current->get_next(i) && current->get_next(i)->hash_key < hash_key)
        {
            current = current->get_next(i);
        }
    }

    current = current->get_next(0);

    while(current && current->hash_key == hash_key)
    {
        // 优化：添加前缀匹配检查
        if(current->value == text)
        {
            found = true;
        }
        current = current->get_next(0);
    }

    return found;
}

// 关键优化：批量更新节点
Node<string>** WuManberSkipList::find_update_nodes(size_t hash_key, bool for_detele)
{
    Node<string>** update = new Node<string>*[max_level + 1];
    Node<string>* current = header;

    for(int i = current_level; i >= 0; i--)
    {
        while(current->get_next(i) && current->get_next(i)->hash_key < hash_key)
        {
            current = current->get_next(i);
        }
        update[i] = current;
    }
    return update;
}

// Wu-Manber专用层级生成算法
int WuManberSkipList::get_random_level()
{
    int level = 1;
    while (level < max_level && (rand() / (RAND_MAX + 1.0)) < probability)
    {
        level++;
    }
    return level;
}

#endif // SKIPLIST_WU_MANBER_H