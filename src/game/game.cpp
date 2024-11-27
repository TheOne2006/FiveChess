#include "game.h"
#include "ai.h"
#include "menu.h"
#include "myIO.h"
#include <conio.h>
#include <cstdlib>
#include <graphics.h>
#include <iostream>
#include <stack>
using namespace myBoard;

// 一些公共变量
myBoard::Board core;
IMAGE chess_board;
std::vector<myButton *> buttons;
// 这一部分是对显示界面的控制
// 重新根据core绘制整个屏幕
void print() {
    point n;
    cleardevice();
    putimage(0, 0, &chess_board);
    // 打印棋子
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
    // 打印菜单
    for (int i = 0; i < buttons.size(); i++)
        buttons[i]->print();
}

// 对界面的一个初始化
void initiation() {
    loadimage(&chess_board, _T("D:/WorkSpace/FiveChess/res/board.jpg"));
    initgraph(535, 555);

    print();
}

void restart() {
    buttons = getAllButton();
    core.reset();
    print();
}

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

bool isOnButton(int dx, int dy) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i]->isBeTouch(dx, dy))
            buttons[i]->leftPut();
    }
    // 点击到按钮，操作结束后需要重新刷新一次屏幕
    print();
}

// 撤回操作与下一步操作，对按钮的一个实现。
// 加入ai后负责对ai核心也进行实现
std::stack<int> actions;
bool undo() {
    int n = core.numPieces;
    if (n <= 1)
        return false;
    actions.push(core.undoState[n - 1]);
    actions.push(core.undoState[n - 2]);
    core.undo(), print();
    return true;
}

bool nextDo() {
    if (!actions.empty()) {
        core.doAction(actions.top()), actions.pop();
        return true;
    }
    return false;
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
            // 点击到同一个点 可以返回
            if (click2.x == click1.x && click2.y == click1.y)
                return click1;
            click1.initiationWithXY(click2.x, click2.y);
            // 点击到不同的点，重新打印边框
            goto loop3;
        } else {
            // 判断是否点击到某个按钮
            if (isOnButton(m.x, m.y)) {
                // 点击到按钮，重新到loop1
                goto loop1;
            } else {
                // 什么都没点击到，继续收集第二次点击信息
                goto loop2;
            }
        }
    } // 判断是否点击到某个按钮
    else {
        isOnButton(m.x, m.y);
        goto loop1;
    }
}

bool isOk(point p) { return core.get(p.x, p.y) == -1; }

// 这一部分是游戏的流程控制
int gameStart() {
    restart();
    bool flag = 0;
    ai::AI pl = ai::AI(1, 1);
    ai::AI pl2 = ai::AI(0, 1);
    // std::vector<int> last = readFromF("my.txt");
    // for(int i = 0; i < last.size(); i++)
    //     core.doAction(last[i]);
    // print();
    int f;
    point p;
    // core.modify(6, 6), pl.modify(6, 6), pl2.modify(6, 6);
    print(), flag = !flag;
    while (true) {
        // 测试人机代码
        p = pl.getNextStep();
        core.modify(p.x, p.y);
        pl.modify(p.x, p.y);
        pl2.modify(p.x, p.y);
        print();
        flag = !flag;

        f = core.win_end();
        if (f == 0) {
            printf("White Win pl2!!\n");
            return win(0);
        } else if (f == 1) {
            printf("Black Win pl!!\n");
            return win(1);
        }

        // // 人工代码
        // point a = getClick(flag);
        // while (!isOk(a))
        //     a = getClick(flag);
        // while (!actions.empty())
        //     actions.pop();
        // core.modify(a.x, a.y);
        // pl.modify(a.x, a.y);
        // pl2.modify(a.x, a.y);
        // print();
        // flag = !flag;

        // f = core.win_end();
        // if (f == 0) {
        //     printf("White Win\n");
        //     return win(0);
        // } else if (f == 1) {
        //     printf("Black Win\n");
        //     return win(1);
        // }
        // 测试人机代码
        p = pl2.getNextStep();
        core.modify(p.x, p.y);
        pl.modify(p.x, p.y);
        pl2.modify(p.x, p.y);
        print();
        flag = !flag;

        f = core.win_end();
        if (f == 0) {
            printf("White Win pl2!!\n");
            return win(0);
        } else if (f == 1) {
            printf("Black Win pl!!\n");
            return win(1);
        }
        // ExMessage m = getmessage(EX_MOUSE);
        // while (!(m.lbutton)) {
        //     m = getmessage(EX_MOUSE);
        // }

        if(core.numPieces > 2) {
            pl.modifyDepth(2);
            pl2.modifyDepth(2);
        }
        if(core.numPieces > 27) {
            pl.modifyDepth(1);
            pl2.modifyDepth(1);
        }

    }
}