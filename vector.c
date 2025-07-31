// 実行
// gcc -O3 -fno-inline -I./src vector.c src/clock64.o -o vector

#include <stdio.h>
#include <stdlib.h>
#include "clock.h"

#define N 2048

int src[N][N];
int dst[N][N];

int global_side_effect = 0; // 最適化防止用のグローバル変数

#define B 10  // ブロックサイズ最適値

void vectored_rotate(int n, int src[n][n], int dst[n][n]) {
    int i, j, i1;
    for (i = 0; i < n; i += B)
        for (j = 0; j < n; j += B)
            for (i1 = i; i1 < i + B && i1 < n; i1++) {
                // j1ループを展開（B=10）
                dst[n - 1 - (j + 0)][i1] = src[i1][j + 0];
                dst[n - 1 - (j + 1)][i1] = src[i1][j + 1];
                dst[n - 1 - (j + 2)][i1] = src[i1][j + 2];
                dst[n - 1 - (j + 3)][i1] = src[i1][j + 3];
                dst[n - 1 - (j + 4)][i1] = src[i1][j + 4];
                dst[n - 1 - (j + 5)][i1] = src[i1][j + 5];
                dst[n - 1 - (j + 6)][i1] = src[i1][j + 6];
                dst[n - 1 - (j + 7)][i1] = src[i1][j + 7];
                if (j + 8 >= n) break;
                dst[n - 1 - (j + 8)][i1] = src[i1][j + 8];
                dst[n - 1 - (j + 9)][i1] = src[i1][j + 9];
            }

    global_side_effect += dst[0][0]; // 最適化防止用の副作用
}


void initialize_matrix(int n, int mat[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = rand() % 1000; // ランダムな初期値（規則性回避）
        }
    }
}

int main() {
    int trials = 10;
    double t;
    int k;
    volatile int result; // to make sure assignment is performed

    initialize_matrix(N, src);
    initialize_matrix(N, dst);

    result = 0;
    vectored_rotate(N, src, dst); // warm up CPU and cache

    for (k= 0; k < trials; k++) {
        start_counter();
        vectored_rotate(N, src, dst);
        t = get_counter();

        // CPE = cycles / (n*n)
        printf("Trial %d: CPE: %.4f\n", k + 1, t / (N * N));
    }

    result = global_side_effect; // 最適化防止用
    return 0;
}
