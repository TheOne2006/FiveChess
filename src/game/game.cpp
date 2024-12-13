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

// 给出棋盘核心
myBoard::Board *getCore() { return &core; }
// 给出现在下的类型
int gameKind = 0;
// 游戏类型0: 双人游戏，1：单人游戏：AI执白棋，2：单人游戏：AI执黑
int getKind() { return gameKind; }

IMAGE chess_board;
std::vector<myButton *> buttons;
ai::AI *aiPlayer;
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
}

// 对界面的一个初始化
void initiation() {
    loadimage(&chess_board, _T("D:/WorkSpace/FiveChess/res/board.png"));
    initgraph(620, 555);
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
// 0表示未点击到，1表示点击到非return，2表示点击到return
int isOnButton(int dx, int dy) {
    bool f1 = false, f2;
    ;
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i]->isBeTouch(dx, dy))
            f2 = buttons[i]->leftPut(), f1 = true;
    }
    // 点击到按钮，操作结束后需要重新刷新一次屏幕
    print();
    if (f1) {
        if (f2)
            return 2;
        else
            return 1;
    } else
        return 0;
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
    core.undo(), print(), aiPlayer->undo();
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
    if (!(m.message == WM_LBUTTONDOWN))
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
            int k = isOnButton(m.x, m.y);
            if (k == 1) {
                // 点击到按钮，重新到loop1
                goto loop1;
            } else if (k == 2) {
                // 点击到return;
                click1.x = -100;
                return click1;
            } else {
                // 什么都没点击到，继续收集第二次点击信息
                goto loop2;
            }
        }
    } // 判断是否点击到某个按钮
    else {
        int k = isOnButton(m.x, m.y);
        if (k == 2) {
            // 点击到return按钮直接返回；
            click1.x = -100;
            return click1;
        } else {
            // 什么都没点击到，继续收集点击信息
            goto loop1;
        }
    }
}

bool isOk(point p) { return core.get(p.x, p.y) == -1; }

int depth = 2; // ai的聪明程度

bool modifyDepth(int d) { depth = d; }

int getDepth() { return depth; }

int aiColor = 1;

int getAIColor() { return aiColor; }

void modifyAi(int a) { aiColor = a; }

// 这一部分是游戏的流程控制
// point的约束，如果point的x为-1代表会退出
int gameSingleStart() {
    restart();
    if (aiColor == BLACK_COLOR)
        gameKind = 2;
    else
        gameKind = 1;
    int flag = 1, f;
    ai::AI pl = ai::AI(aiColor, depth);
    aiPlayer = &pl;
    point p;
    // core.modify(6, 6), pl.modify(6, 6), pl2.modify(6, 6);
    print();
    while (true) {
        if (flag == aiColor) {
            // 人机代码
            p = pl.getNextStep();
            core.modify(p.x, p.y);
            pl.modify(p.x, p.y);
            printf("AI choose %d %d\n", p.x, p.y);
            print(), printEdge(p, aiColor);
            flag = !flag;

            // p = pl.getNextStep();
            // printf("AI choose %d %d\n", p.x, p.y);
            // point a = getClick(flag);
            // if (a.x == -100) {
            //     return -1;
            // }
            // while (!isOk(a)) {
            //     a = getClick(flag);
            //     if (a.x == -100) {
            //         return -1;
            //     }
            // }
            // while (!actions.empty())
            //     actions.pop();
            // core.modify(a.x, a.y);
            // pl.modify(a.x, a.y);
            // print();
            // flag = !flag;
        } else {
            // 人工代码
            point a = getClick(flag);
            if (a.x == -100) {
                return -1;
            }
            while (!isOk(a)) {
                a = getClick(flag);
                if (a.x == -100) {
                    return -1;
                }
            }
            while (!actions.empty())
                actions.pop();
            core.modify(a.x, a.y);
            pl.modify(a.x, a.y);
            print();
            flag = !flag;
        }
        f = core.win_end();
        if (f == 0) {
            printf("White Win\n");
            return win(0);
        } else if (f == 1) {
            printf("Black Win\n");
            return win(1);
        }
    }
}

int gameTwoStart() {
    restart(), gameKind = 0;
    int flag = 1, f;
    point p;
    // core.modify(6, 6), pl.modify(6, 6), pl2.modify(6, 6);
    print();
    while (true) {
        // 人工代码
        point a = getClick(flag);
        if (a.x == -100) {
            return -1;
        }
        while (!isOk(a)) {
            a = getClick(flag);
            if (a.x == -100) {
                return -1;
            }
        }
        while (!actions.empty())
            actions.pop();
        core.modify(a.x, a.y);
        print();
        flag = !flag;

        f = core.win_end();
        if (f == 0) {
            printf("White Win\n");
            return win(0);
        } else if (f == 1) {
            printf("Black Win\n");
            return win(1);
        }
    }
}

// 游戏类型0: 双人游戏，1：单人游戏：AI执白棋，2：单人游戏：AI执黑
int gameLoadStart(std::vector<int> steps) {
    restart();
    if (steps[0] == 0) {
        int flag = 1, f;
        for (int i = 1; i < steps.size(); i++)
            core.doAction(steps[i]), flag = !flag;
        print();
        while (true) {
            // 人工代码
            point a = getClick(flag);
            if (a.x == -100) {
                return -1;
            }
            while (!isOk(a)) {
                a = getClick(flag);
                if (a.x == -100) {
                    return -1;
                }
            }
            while (!actions.empty())
                actions.pop();
            core.modify(a.x, a.y);
            print();
            flag = !flag;

            f = core.win_end();
            if (f == 0) {
                printf("White Win\n");
                return win(0);
            } else if (f == 1) {
                printf("Black Win\n");
                return win(1);
            }
        }
    } else {
        if (steps[0] == 1) {
            aiColor = WHITE_COLOR;
        } else {
            aiColor = BLACK_COLOR;
        }
        // 人对战AI
        if (aiColor == WHITE_COLOR) {
            gameKind = 1;
        } else {
            gameKind = 2;
        }
        int flag = 1, f;
        ai::AI pl = ai::AI(aiColor, depth);
        aiPlayer = &pl;
        point p;
        // core.modify(6, 6), pl.modify(6, 6), pl2.modify(6, 6);
        for (int i = 1; i < steps.size(); i++) {
            flag = !flag, core.doAction(steps[i]), pl.doAction(steps[i]);
        }
        print();
        while (true) {
            if (flag == aiColor) {
                // 人机代码
                p = pl.getNextStep();
                core.modify(p.x, p.y);
                pl.modify(p.x, p.y);
                print();
                flag = !flag;

                f = core.win_end();
            } else {
                // 人工代码
                point a = getClick(flag);
                if (a.x == -100) {
                    return -1;
                }
                while (!isOk(a)) {
                    a = getClick(flag);
                    if (a.x == -100) {
                        return -1;
                    }
                }
                while (!actions.empty())
                    actions.pop();
                core.modify(a.x, a.y);
                pl.modify(a.x, a.y);
                print();
                flag = !flag;
            }
            f = core.win_end();
            if (f == 0) {
                printf("White Win\n");
                return win(0);
            } else if (f == 1) {
                printf("Black Win\n");
                return win(1);
            }
        }
    }
}