#include "game.h"
#include <graphics.h>
#include <conio.h>
#include <iostream>
using namespace myBoard;

// 一些公共变量
myBoard::Board core;
IMAGE chess_board;

// 这一部分是对显示界面的控制
// 重新根据core绘制整个屏幕
void print() {
    point n;
    cleardevice(), putimage(0, 0, &chess_board);
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++) {
            n.initiationWithXY(i, j);
            if (core.get(i, j) == 0) {
                setfillcolor(WHITE);
                solidcircle(n.dx, n.dy, n.R);
            } else if (core.get(i, j) == 1) {
                setfillcolor(BLACK);
                solidcircle(n.dx, n.dy, n.R);
            }
        }
}

// 对界面的一个初始化
void initiation() {
    loadimage(&chess_board, _T("D:/WorkSpace/FiveChess/res/board.jpg"));
    initgraph(535, 535);

    print();
}

void restart() { core.reset(), print(); }

// 打印第一次点击的框框
void printEdge(point p, int flag) {
    print();
    setlinestyle(PS_DASH, 2);
    setlinecolor(DARKGRAY);
    rectangle(p.dx - p.edgeDis, p.dy + p.edgeDis, p.dx + p.edgeDis,
              p.dy - p.edgeDis);
}

// 弹出胜利的消息框
int win(bool flag) {
    HWND win = GetHWnd();
    if (flag)
        return MessageBox(win, _T("black win!!"), _T("Game Over!"),
                          MB_RETRYCANCEL);
    else
        return MessageBox(win, _T("white win!!"), _T("Game Over!"),
                          MB_RETRYCANCEL);
}

// 这一部分是对下棋的控制,属于键盘的操纵界面
point getClick(int flag) {
    point click1, click2;
    ExMessage m;
loop1:
    m = getmessage(EX_MOUSE);
    if (!m.lbutton) 
        goto loop1;
    // 若第一次点击成功命中某个坐标
    if (click1.initiationWithD(m.x, m.y)) {
    loop3:
        printEdge(click1, flag);
    loop2:
        m = getmessage(EX_MOUSE);
        if (!m.lbutton)
            goto loop2;
        if (click2.initiationWithD(m.x, m.y)) {
            if (click2.x == click1.x && click2.y == click1.y)
                return click1;
            click1.initiationWithXY(click2.x, click2.y);
            goto loop3;
        } else
            goto loop2;
    } else
        goto loop1;
}

bool isOk(point p) { return core.get(p.x, p.y) == -1; }

// 这一部分是游戏的流程控制
int gameStart() {
    restart();
    bool flag = 0;
    while (true) {
        point a = getClick(flag);
        while (!isOk(a))
            a = getClick(flag);
        core.modify(a.x, a.y);
        print();
        flag = !flag;
        int f = core.win_end();
        if (f == 0) {
            printf("White Win\n");
            return win(0);
        } else if (f == 1) {
            printf("Black Win\n");
            return win(1);
        }
    }
}