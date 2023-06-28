
/** @file command.h */
#pragma once

#define ARGC_MAX 32

/** @ingroup shell
 * @struct args
 * @brief 引数構造体
 */
struct args {
    char *argv[ARGC_MAX];   /**< 引数の配列 */
    int argc;               /**< 引数の数   */
};

/** @ingroup shell
 * @struct command
 * @brief コマンド構造体
 */
struct command {
    const char *name;               /**< コマンド名 */
    const char *help;               /**< コマンドhelp文字列 */
    void (*run)(struct args *args); /**< コマンド実行関数 */
};

void run_command(struct args *args);
