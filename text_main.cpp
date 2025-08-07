#include "./skiplist/include/skiplist_1.h"
#include <iostream>

int main() {
    WuManberSkipList wm_skiplist(10, 3); // 最大10层，前缀长度3
    
    // 添加模式串
    wm_skiplist.insert_pattern("algorithm");
    wm_skiplist.insert_pattern("alligator");
    wm_skiplist.insert_pattern("alternative");
    wm_skiplist.insert_pattern("alfabet");
    
    // 优化跳表结构
    wm_skiplist.optimize_for_wumanber();

    // 搜索匹配
    if (wm_skiplist.search_exact_match("alfabet")) {
        std::cout << "string: ";
        std::cout << std::endl;
    }
    
    return 0;
}