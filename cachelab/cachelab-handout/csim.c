#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>

struct CacheLine
{
    int time;  // LRU用参数
    int valid; // 有效位
    int tag;   // 标记位
    // 块号部分置空
};
typedef struct CacheLine CacheLine;
typedef CacheLine **Cache;

int s; // 组索引位数
int E; // 每组的行数
int b; // 块内偏移位数
FILE *traceFile;
int verboseFlag = 0;

void readArgs(int argc, char *argv[]);
Cache initCache();

int main(int argc, char *argv[])
{
    readArgs(argc, argv);

    Cache cache = initCache(s, E);

    char type;
    int address, size;
    int hits = 0, misses = 0, evictions = 0;

    while (fscanf(traceFile, "%c %lx,%d\n", &type, &address, &size) != EOF)
    {
        if (type == 'I' || type == ' ') // 忽略 I 和 空格
        {
            continue;
        }
        printf("信息: %c %x,%d\t", type, address, size);

        int addr = address >> b;              // 去掉块内偏移位
        int tag = addr >> s;                  // 取出标记位
        int setIndex = addr & ((1 << s) - 1); // 取出组索引位
        for (int i = 0; i < E; i++)
        {
            if (cache[setIndex][i].valid && cache[setIndex][i].tag == tag) // 命中
            {
                hits++;
                cache[setIndex][i].time = 0;
                if (verboseFlag)
                {
                    printf("hits", address);
                }
                break;
            }
            else if (!cache[setIndex][i].valid) // 未命中且组内有空行
            {
                misses++;
                cache[setIndex][i].valid = 1;
                cache[setIndex][i].tag = tag;
                cache[setIndex][i].time = 0;
                if (verboseFlag)
                {
                    printf("miss", address);
                }
                break;
            }
            else if (i == E - 1) // 未命中且组内无空行, 进行替换
            {
                misses++;
                evictions++;
                // LRU, 找到最大的time
                int maxTime = cache[setIndex][0].time;
                int maxIndex = 0;
                for (int j = 1; j < E; j++)
                {
                    if (cache[setIndex][j].time > maxTime)
                    {
                        maxTime = cache[setIndex][j].time;
                        maxIndex = j;
                    }
                    printf("cache[setIndex][%d].time = %d\n", j, cache[setIndex][j].time);
                }
                cache[setIndex][maxIndex].tag = tag;
                cache[setIndex][maxIndex].time = 0;
                if (verboseFlag)
                {
                    printf("miss eviction", address);
                }
            }
            // LRU, 其他行的time++
            cache[setIndex][i].time++;
        }
        if (type == 'M')
        {
            hits++;
            printf(" hits");
        }
        printf("\n");
    }

    printSummary(hits, misses, evictions);
    return 0;
}

char *optarg;
void readArgs(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printf("-h: Optional help flag that prints usage info\n");
            printf("-v: Optional verbose flag that displays trace info\n");
            printf("-s <s>: Number of set index bits (S = 2s is the number of sets)\n");
            printf("-E <E>: Associativity (number of lines per set)\n");
            printf("-b <b>: Number of block bits (B = 2b is the block size)\n");
            printf("-t <tracefile>: Name of the valgrind trace to replay\n");

            exit(0);
        case 'v':
            printf("Verbose mode on\n");
            verboseFlag = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            traceFile = fopen(optarg, "r");
            break;
        default:
            fprintf(stderr, "Usage: %s [-s <s>] [-E <E>] [-b <b>] [-t <tracefile>]\n", argv[0]);
            exit(1);
        }
    }
}

Cache initCache(int s, int E)
{
    int S = 1 << s;
    Cache cache = (Cache)malloc(sizeof(CacheLine *) * S);
    for (int i = 0; i < S; i++)
    {
        cache[i] = (CacheLine *)malloc(sizeof(CacheLine) * E);
        for (int j = 0; j < E; j++)
        {
            cache[i][j].time = 0;
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
        }
    }
    return cache;
}