/*
 * CS:APP Cache Lab - Part A
 *
 * Name: flazi
 * Login ID: flazi
 *
 * Description:
 * A cache simulator using the LRU replacement policy.
 */
#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



/*
* 模拟缓存访问的结构体定义
*
*/
// 因为没有trace文件没有传递b域，所以不需要b字段
struct cache_line {
    int valid; // 有效位
    unsigned long tag; // 标签
    unsigned long last_used; // LRU计数器
};

struct cache_set {
    struct cache_line *lines; // 缓存行数组
};

struct cache {
    struct cache_set *sets; // 缓存组数组
    int S; // 缓存组数
    int E; // 每组的缓存行数
};



int main(int argc, char *argv[])
{
    // 读取命令行参数
    // 解析参数，设置缓存的大小、组数、块大小等
    char *s = argv[2];
    char *E = argv[4];
    char *b = argv[6];
    printf("%s %s %s\n", s, E, b);

    //初始化缓存结构体
    struct cache my_cache;
    my_cache.S = pow(2, atoi(s));
    my_cache.E = atoi(E);

    my_cache.sets = (struct cache_set *)malloc(my_cache.S * sizeof(struct cache_set));
    for (int i = 0; i < my_cache.S; i++) {
        my_cache.sets[i].lines = (struct cache_line *)malloc(my_cache.E * sizeof(struct cache_line));
        for (int j = 0; j < my_cache.E; j++) {
            my_cache.sets[i].lines[j].valid = 0;
            my_cache.sets[i].lines[j].tag = 0;
            my_cache.sets[i].lines[j].last_used = 0;
        }
    }
    //读取trace文件，模拟缓存访问
    FILE *trace_file = fopen(argv[8], "r");
    if (trace_file == NULL) {
        fprintf(stderr, "Error opening trace file: %s\n", argv[8]);
        return 1;
    }

    //统计数字
    int hit_count = 0;
    int miss_count = 0;
    int eviction_count = 0;

    //读取一行
    char operation;
    unsigned long address;
    int size;

    //全局计数器，没发生一次访问+1
    unsigned long clock = 0;

    while (fscanf(trace_file, " %c %lx,%d", &operation, &address, &size) == 3) {
        clock++; // 每次访问增加计数器
        if (operation == 'I') {
            continue; // 忽略指令访问
        }
        //解析地址，计算组索引和标签
        unsigned long set_index = (address >> atoi(b)) & (my_cache.S - 1);
        unsigned long tag = address >> (atoi(b) + atoi(s));
        // 根据组索引和标签访问缓存，更新命中次数、缺失次数和替换次数
        // 这里需要实现具体的缓存访问逻辑
        struct cache_set *set = &my_cache.sets[set_index];
        int hit = 0;
        for (int j = 0; j < my_cache.E; j++) {
            if (set->lines[j].valid && set->lines[j].tag == tag) {
                hit = 1;
                if (operation == 'M') {
                    hit_count++; // 对于修改操作，命中两次
                }
                hit_count++;

                set->lines[j].last_used = clock; // 更新最近一次使用
                break;
            }
        }
        if (!hit) {
            miss_count++;
            // 寻找一个可以替换的行
            int replace_index = -1;
            for (int j = 0; j < my_cache.E; j++) {
                if (!set->lines[j].valid) {
                    replace_index = j;
                    break;
                }
            }
            if (replace_index == -1) {
                // 找到LRU行进行替换
                replace_index = 0;
                for (int j = 1; j < my_cache.E; j++) {
                    if (set->lines[j].last_used < set->lines[replace_index].last_used) {
                        replace_index = j;
                    }
                }
                eviction_count++;//未命中，并且需要进行替换的行
                if (operation == 'M') {
                    hit_count++; // 对于修改操作，命中两次
                }
            }else{
                if (operation == 'M') {
                    hit_count++; // 未命中但是有空行
                }
            }
            // 更新被替换的行
            set->lines[replace_index].valid = 1;
            set->lines[replace_index].tag = tag;
            set->lines[replace_index].last_used = clock;
        }
    }


    // 打印统计命中次数、缺失次数和替换次数
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
