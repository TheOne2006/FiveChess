#include <game.h>
#include <iostream>
#include "graphics.h"
#include "conio.h"
#include <point.h>
#include <ctime>
#include <random>
int main() {
    srand((unsigned)std::time(NULL));
    initiation();
    while (true) {
        if (gameStart() == IDCANCEL)
            break;
    }
    closegraph(); // 关闭绘图窗口
    return 0;
}