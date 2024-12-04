#include "conio.h"
#include "graphics.h"
#include <Shlwapi.h>
#include <ctime>
#include <game.h>
#include <iostream>
#include <myIO.h>
#include <point.h>
#include <random>
#include <start.h>
#include <unistd.h>
int main() {
    // srand((unsigned)std::time(NULL));
    // initiation();
    // while (true) {
    //     if (gameStart() == IDCANCEL)
    //         break;
    // }
    // closegraph(); // 关闭绘图窗口
    my_io::initiation();
    my_start::start();
    return 0;
}