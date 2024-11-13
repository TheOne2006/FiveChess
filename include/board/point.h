#ifndef MY_POINT
#define MY_POINT

#define BOARD_LEN 15
// 点对象，表示某一个点。
namespace myBoard {
struct point {
    const int startX = 22, startY = 22, dis = 35, R = 8, edgeDis = 10;
    int x = -1, y = -1, p = 0; // p为一个点的int位置
    int dx, dy;
    int color;

    // 构造函数，根据坐标确定棋盘上的位置
    bool initiationWithXY(int a, int b);
    
    // 构造函数2，根据一个点的int位置构造点。
    // 约定：某个点的表示的一种方式 (x) * BoardLen + (y)
    bool initiationWithP(int p);

    // 等于函数重载
    point operator=(const point &b);
    bool initiationWithD(int dx, int dy);
};
} // namespace board
#endif