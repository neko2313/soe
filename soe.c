/**
 * エラトステネスの篩
 * INT_MAX=2147483647(32bit)を想定
 * 細かいエラー判定はしていません
 */
// $Id: soe.c,v 1.1.1.1 2017/03/17 12:20:54 kondo Exp $
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
/*
 * us: ./a.out max メモリが許すならかなりいける＾＾；
 * 1億の配列で４００ＭＢ程度・・・
 */
/* 速度テスト用：素数表示なし実行時間と個数のみ(10億の時↓)
Time:23.318824

*** prime count: 50847534 ***

環境
linux mint 18.3
clang version 8.0.0 (trunk 341621)
AMD Ryzen 5 1600x 3.6〜4.0GHz 
*/
//
void SieveOfEratosthenes(long arry[], long siz);
long argCheck(int agc, char *agv[]);
//
//#define CLOCK_TEST  // 速度計測用
/* ----- Caution！ -----
 * SIZ_LIMはMAX_INT/2(≒1073740000)以上にはしないこと
 * 篩の中でi+iがintの範囲を超えてしまうため正常に動きません
 */
#ifdef CLOCK_TEST
#include <time.h>          // for clock()
#define SIZ_LIM 1000000001  // 最大１０億まで
#else
// 最大値はとりあえずshort(=32767)にしてある。
#define SIZ_LIM SHRT_MAX
#endif
/*
 * エラトステネスの篩本体 O(n log log n)
 */
void SieveOfEratosthenes(long arry[], long siz)
{
    long lim = (long)(ceil(sqrt(siz+2)));  // 最大値の平方根+αまで調べればいい
    for (long i = 2; i < lim; i++) {
        if (arry[i]) {
            long tmp = arry[i];  // 素数を取り出す
            for (long j = i + i; j < siz; j += tmp) {
                arry[j] = 0;  // 素数の倍数をクリア
            }
        }
    }
}
/*
 * 実行時引数チェック
 */
long argCheck(int agc, char *agv[])
{
    // パラメータ判定
    if (agc != 2) {
        fputs("素数を求める最大値をパラメータで指定してください。\n", stderr);
        return 0;
    }
    // パラメータから最大数に変換(数値のみかどうかの判断はしていない)
    long max = strtol(agv[1], NULL, 10);
    /* ./a.out 12abとかってやられると問題 ./a.out abcは0になるから大丈夫d^^ */
    if (max < 3) {
        fputs("最大値の指定は半角数字で３以上を指定してください。\n", stderr);
        return 1;
    }
    // 最大値判定
    if (max > SIZ_LIM) {
        fputs("最大値が大きすぎます。\n", stderr);
        return 2;
    }
    //
    return max;
}
/**
 *  main
 *
 */
int main(int agc, char *agv[])
{
    long max = argCheck(agc, agv);
    if (max < 3) {
        return (int)(max + 1);  // エラーは１～３が返る
    }
    // メモリ確保
    long *ptr = (long *)(calloc((size_t)max, sizeof(long)));
    if (ptr == NULL) {
        fputs("メモリの確保に失敗しました。\n", stderr);
        return 4;
    }
    long *tp = ptr;
    // 初期化:確保した領域に数値を入れる
    // フラグにしないのはあとで使うから…
    for (long i = 0; i < max; i++) {
        *tp++ = i;
    }
    // １は素数じゃない・・・
    // １と自分自身以外に正の約数を持たない自然数で、
    // １でない数のことである。 ・・・・らしい
    ptr[1] = 0;  // 上のループを２から始めたほうがいいかも・・・
#ifdef CLOCK_TEST
    clock_t start = clock();
#endif
    // 篩に掛ける
    SieveOfEratosthenes(ptr, max);
#ifdef CLOCK_TEST
    clock_t end = clock();
    printf("Time:%f\n", (double)(end - start) / CLOCKS_PER_SEC);
#endif
    tp = ptr;
#ifndef CLOCK_TEST
    int col_siz = (int)(log10((double)max));
    if (col_siz < 3) {
        col_siz = 3;
    }
#endif
    // 結果表示
    int cnt = 0;  // 素数を数える
    for (long i = 0; i < max; i++) {
        if (*tp != 0) {
            cnt++;
#ifndef CLOCK_TEST
            printf("%*ld ", col_siz, *tp);
            if ((cnt & 7) == 0) {
                putchar('\n');
            }
#endif
        }
        tp++;
    }
    // メモリ開放
    free(ptr);
    // 素数の数を表示
    printf("\n*** prime count: %d ***\n", cnt);
    fflush(stdout);  // おまじない＾＾；
    //
    return 0;
}
