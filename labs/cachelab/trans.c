/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 * 
 * Name: flazi
 * Login ID: flazi
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void trans(int M, int N, int A[N][M], int B[M][N]);
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
    int i, j, k,l;
    int a0, a1, a2, a3, a4, a5, a6, a7;
    if (M == 32 && N == 32) {
        // 处理8x8的块+8个局部变量
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                for (k = i; k < i + 8 && k < N; k++) {
                    a0 = A[k][j];
                    a1 = A[k][j + 1];
                    a2 = A[k][j + 2];
                    a3 = A[k][j + 3];
                    a4 = A[k][j + 4];
                    a5 = A[k][j + 5];
                    a6 = A[k][j + 6];
                    a7 = A[k][j + 7];

                    B[j][k] = a0;
                    B[j + 1][k] = a1;
                    B[j + 2][k] = a2;
                    B[j + 3][k] = a3;
                    B[j + 4][k] = a4;
                    B[j + 5][k] = a5;
                    B[j + 6][k] = a6;
                    B[j + 7][k] = a7;
                }
            }
        }
    } else if (M == 64 && N == 64) {

        for (i = 0; i < 64; i += 8) {
            for (j = 0; j < 64; j += 8) {

                /* 第一步：处理 A 的左上和右上 4×4 */

                for (k = i; k < i + 4; k++) {
                    a0 = A[k][j];
                    a1 = A[k][j + 1];
                    a2 = A[k][j + 2];
                    a3 = A[k][j + 3];
                    a4 = A[k][j + 4];
                    a5 = A[k][j + 5];
                    a6 = A[k][j + 6];
                    a7 = A[k][j + 7];

                    /* 左上：正常转置 */
                    B[j][k] = a0;
                    B[j + 1][k] = a1;
                    B[j + 2][k] = a2;
                    B[j + 3][k] = a3;

                    /* 右上：暂存在 B 的右上区域 */
                    B[j][k + 4] = a4;
                    B[j + 1][k + 4] = a5;
                    B[j + 2][k + 4] = a6;
                    B[j + 3][k + 4] = a7;
                }

                /* 第二步：处理 A 的左下，并搬运暂存数据 */

                for (k = 0; k < 4; k++) {
                    /* 读取 A 左下的四个元素 */
                    a0 = A[i + 4][j + k];
                    a1 = A[i + 5][j + k];
                    a2 = A[i + 6][j + k];
                    a3 = A[i + 7][j + k];

                    /* 取出之前暂存在 B 右上的数据 */
                    a4 = B[j + k][i + 4];
                    a5 = B[j + k][i + 5];
                    a6 = B[j + k][i + 6];
                    a7 = B[j + k][i + 7];

                    /* A 左下 -> B 右上 */
                    B[j + k][i + 4] = a0;
                    B[j + k][i + 5] = a1;
                    B[j + k][i + 6] = a2;
                    B[j + k][i + 7] = a3;

                    /* 暂存数据 -> B 左下 */
                    B[j + 4 + k][i] = a4;
                    B[j + 4 + k][i + 1] = a5;
                    B[j + 4 + k][i + 2] = a6;
                    B[j + 4 + k][i + 3] = a7;
                }

                /* 第三步：处理 A 的右下 4×4 */

                for (k = i + 4; k < i + 8; k++) {
                    a0 = A[k][j + 4];
                    a1 = A[k][j + 5];
                    a2 = A[k][j + 6];
                    a3 = A[k][j + 7];

                    B[j + 4][k] = a0;
                    B[j + 5][k] = a1;
                    B[j + 6][k] = a2;
                    B[j + 7][k] = a3;
                }
            }
        }
    

        
    }else if (M == 61 && N == 67) {
        int blockH = 17;
        int blockW = 8;

        for (i = 0; i < N; i += blockH) {
            for (j = 0; j < M; j += blockW) {

                for (l = j; l < j + blockW && l < M; l++) {
                    for (k = i; k < i + blockH && k < N; k++) {
                        B[l][k] = A[k][l];
                    }
                }
            }
        }
        return;
    }
    

    return;
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            if(i<56 && j<56){
                continue;
            }
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
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

