/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

void tran32_32(int M, int N, int A[N][M], int B[M][N]);
void tran64_64(int M, int N, int A[N][M], int B[M][N]);
void tran61_67(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    // s=5, E=1, b=5, 一个block大小为32B, 存储8个int
    // A=0010c100, B=0014c100, 访问同一个block时一定会冲突
    // 所以未减少miss对角线一定要跳过
    if (M == 32 && N == 32)
    {
        tran32_32(M, N, A, B);
    }
    else if (M == 64 && N == 64)
    {
        tran64_64(M, N, A, B);
    }
    else if (M == 61 && N == 67)
    {
        tran61_67(M, N, A, B);
    }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

void tran32_32(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, cnt;
    int a1, a2, a3, a4, a5, a6, a7, a8;
    // 对A一次读入8个int(一个lock), 循环八次, 写入B的8个block
    for (i = 0; i < 32; i += 8)
    {
        for (j = 0; j < 32; j += 8)
        {
            for (cnt = 0; cnt < 8; cnt++, i++)
            {
                // 一次读取8个int
                a1 = A[i][j];
                a2 = A[i][j + 1];
                a3 = A[i][j + 2];
                a4 = A[i][j + 3];
                a5 = A[i][j + 4];
                a6 = A[i][j + 5];
                a7 = A[i][j + 6];
                a8 = A[i][j + 7];
                B[j][i] = a1;
                B[j + 1][i] = a2;
                B[j + 2][i] = a3;
                B[j + 3][i] = a4;
                B[j + 4][i] = a5;
                B[j + 5][i] = a6;
                B[j + 6][i] = a7;
                B[j + 7][i] = a8;
            }
            i -= 8;
        }
    }
}

void tran64_64(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, step;
    int x1, x2, x3, x4, x5, x6, x7, x8;
    for (i = 0; i < N; i += 8)
    {
        for (j = 0; j < M; j += 8)
        {
            // 读A上部, 转置B左上角, 将B左下角转置结果暂存到B右上角
            for (step = 0; step < 4; step++)
            {
                x1 = A[i + step][j];
                x2 = A[i + step][j + 1];
                x3 = A[i + step][j + 2];
                x4 = A[i + step][j + 3];
                x5 = A[i + step][j + 4];
                x6 = A[i + step][j + 5];
                x7 = A[i + step][j + 6];
                x8 = A[i + step][j + 7];
                // B左上角转置
                B[j][i + step] = x1;
                B[j + 1][i + step] = x2;
                B[j + 2][i + step] = x3;
                B[j + 3][i + step] = x4;
                // 转置, 保存到B右上角
                B[j][i + step + 4] = x5;
                B[j + 1][i + step + 4] = x6;
                B[j + 2][i + step + 4] = x7;
                B[j + 3][i + step + 4] = x8;
            }
            // 读A左下角, 转置到B右上角, 将B右上角转置结果存到B左下角(同时更新cache)
            for (step = 0; step < 4; step++)
            {
                // 暂存B右上角第step行
                x1 = B[j + step][i + 4];
                x2 = B[j + step][i + 5];
                x3 = B[j + step][i + 6];
                x4 = B[j + step][i + 7];

                // B右上角转置(对A左下角读第step列, 对B按行写, 完成B上半部分的转置)
                x5 = A[i + 4][j + step];
                x6 = A[i + 5][j + step];
                x7 = A[i + 6][j + step];
                x8 = A[i + 7][j + step];
                B[j + step][i + 4] = x5;
                B[j + step][i + 5] = x6;
                B[j + step][i + 6] = x7;
                B[j + step][i + 7] = x8;
                // 将暂存的B右上角第step行保存到B左下角(更新cache)
                B[j + step + 4][i] = x1;
                B[j + step + 4][i + 1] = x2;
                B[j + step + 4][i + 2] = x3;
                B[j + step + 4][i + 3] = x4;
            }

            for (step = 0; step < 4; step++)
            {
                // 保存A右下角
                x1 = A[i + step + 4][j + 4];
                x2 = A[i + step + 4][j + 5];
                x3 = A[i + step + 4][j + 6];
                x4 = A[i + step + 4][j + 7];
                // B右下角转置
                B[j + 4][i + step + 4] = x1;
                B[j + 5][i + step + 4] = x2;
                B[j + 6][i + step + 4] = x3;
                B[j + 7][i + step + 4] = x4;
            }
            // TODO 对角线的块处理不够好(因为此时A和B的块冲突)
        }
    }
}

void tran61_67(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k, h;
    for (i = 0; i < N; i += 16)
    {
        for (j = 0; j < M; j += 16)
        {
            for (k = i; k < i + 16 && k < N; k++)
            {
                for (h = j; h < j + 16 && h < M; h++)
                {
                    B[h][k] = A[k][h];
                }
            }
        }
    }
}

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    // registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
