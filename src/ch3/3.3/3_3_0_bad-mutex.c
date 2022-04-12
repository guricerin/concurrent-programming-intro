#include <stdbool.h>

// 共有変数
bool lock = false;

// このやり方では、複数のプロセスがクリティカルセクションを同時に実行する可能性がある。
void some_func()
{
retry:
    if (!lock)
    {
        // lock = true にセットする間に、他のプロセスが false なままの lock を読み込む恐れがある。
        lock = true; // ロック獲得
        // クリティカルセクション
    }
    else
    {
        goto retry;
    }
    lock = false; // ロック解放
}
