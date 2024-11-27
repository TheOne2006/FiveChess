#include <iostream>
#include <point.h>

using namespace myBoard;

bool point::initiationWithXY(int a, int b) {
    x = a, y = b;
    dx = startX + dis * (x);
    dy = startY + dis * (y);
    p = BOARD_LEN * (x) + y;
    return 1;
}

bool point::initiationWithD(int px, int py) {
    int sx, sy;
    dx = px, dy = py;
    sx = (dx - startX) / dis + 1;
    sy = (dy - startY) / dis + 1;

    x = sx + ((dx - startX - (sx - 1) * dis) * 2 > dis);
    y = sy + ((dy - startY - (sy - 1) * dis) * 2 > dis);

    if (std::abs(startX + dis * (x - 1) - dx) > edgeDis)
        return 0;
    if (std::abs(startY + dis * (y - 1) - dy) > edgeDis)
        return 0;
    std::printf("%d %d %d %d\n", dx, dy, x, y);
    x--, y--;
    if(x >= 15 || y >= 15 || x < 0 || y < 0) return false;
    initiationWithXY(x, y);
    return 1;
}

bool point::initiationWithP(int p) {
    x = p / BOARD_LEN, y = p % BOARD_LEN;
    initiationWithXY(x, y);
}

point& point::operator=(const point &b) {
    x = b.x, y = b.y;
    dx = b.dx, dy = b.dy;
    p = b.p;
}