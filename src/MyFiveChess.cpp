﻿#include <MyFiveChess.h>

// 一些公共变量
board core;
IMAGE chess_board;

// 这一部分是维护整个棋盘的内部核心数据
// 判断某两个点的颜色是否相同
bool board::judge_dire(int x, int y, int i, int j) {
    if ((x + i > 15) || (y + i > 15) || (x + i < 1) || (y + i < 1))
        return 0;
    else
        return board[x][y] == board[x + i][y + j];
}

// 判断某个点是否为胜子
bool board::judge_point(int x, int y) {
    bool flag = true;
    // left
    for (int i = 1; i <= 4; i++)
        if (!judge_dire(x, y, i, 0)) {
            flag = false;
            break;
        }
    if (flag)
        return true;
    // up
    flag = true;
    for (int i = 1; i <= 4; i++)
        if (!judge_dire(x, y, 0, i)) {
            flag = false;
            break;
        }
    if (flag)
        return true;

    // right
    flag = true;
    for (int i = 1; i <= 4; i++)
        if (!judge_dire(x, y, -i, 0)) {
            flag = false;
            break;
        }
    if (flag)
        return true;

    // down
    flag = true;
    for (int i = 1; i <= 4; i++)
        if (!judge_dire(x, y, 0, -i)) {
            flag = false;
            break;
        }
    if (flag)
        return true;

    // upright
    flag = true;
    for (int i = 1; i <= 4; i++)
        if (!judge_dire(x, y, i, i)) {
            flag = false;
            break;
        }
    if (flag)
        return true;

    // downright
    flag = true;
    for (int i = 1; i <= 4; i++)
        if (!judge_dire(x, y, i, -i)) {
            flag = false;
            break;
        }
    if (flag)
        return true;

    // upleft
    flag = true;
    for (int i = 1; i <= 4; i++)
        if (!judge_dire(x, y, -i, i)) {
            flag = false;
            break;
        }
    if (flag)
        return true;

    // downleft
    flag = true;
    for (int i = 1; i <= 4; i++)
        if (!judge_dire(x, y, -i, -i)) {
            flag = false;
            break;
        }
    if (flag)
        return true;

    return false;
}

int board::judge() {
    for (int i = 1; i <= 15; i++)
        for (int j = 1; j <= 15; j++) {
            if (!board[i][j])
                continue;
            else if (judge_point(i, j))
                return board[i][j];
        }
    return 0;
}

int board::modify(int x, int y, int k) {
    if (board[x][y])
        return false;
    else
        board[x][y] = k;
    return true;
}

// 这一部分是为了在棋盘核心和图像间建立坐标联系
bool point::initiation() {
    if (x) {
        dx = startX + dis * (x - 1);
        dy = startY + dis * (y - 1);
        return 1;
    } else {
        int sx, sy;
        sx = (dx - startX) / dis + 1;
        sy = (dy - startY) / dis + 1;

        x = sx + ((dx - startX - (sx - 1) * dis) * 2 > dis);
        y = sy + ((dy - startY - (sy - 1) * dis) * 2 > dis);

        if (abs(startX + dis * (x - 1) - dx) > edgeDis)
            return 0;
        if (abs(startY + dis * (y - 1) - dy) > edgeDis)
            return 0;
        printf("%d %d %d %d\n", dx, dy, x, y);
        return 1;
    }
}

// 这一部分是对显示界面的控制
// 重新根据core绘制整个屏幕
void print() {
    point n;
    cleardevice(), putimage(0, 0, &chess_board);
    for (int i = 1; i <= 15; i++)
        for (int j = 1; j <= 15; j++) {
            n.x = i, n.y = j, n.initiation();
            if (core.board[i][j] == 1) {
                setfillcolor(WHITE);
                solidcircle(n.dx, n.dy, n.R);
            } else if (core.board[i][j] == 2) {
                setfillcolor(BLACK);
                solidcircle(n.dx, n.dy, n.R);
            }
        }
}

// 对界面的一个初始化
void initiation() {
    loadimage(&chess_board,
            _T("D:/WorkSpace/MyFiveChess/x64./Debug/pictures/board.jpg"));
    initgraph(535, 535);
	
	for (int i = 1; i <= 15; i++)
        for (int j = 1; j <= 15; j++)
            core.board[i][j] = 0;
    print();
}

void restart() {
	for (int i = 1; i <= 15; i++)
        for (int j = 1; j <= 15; j++)
            core.board[i][j] = 0;
    print();
}

// 打印第一次点击的框框
void printEdge(point p, int flag) {
    print();
    setlinestyle(PS_DASH, 2);
    if (!flag)
        setlinecolor(LIGHTGRAY);
    else
        setlinecolor(DARKGRAY);
    p.initiation();
    rectangle(p.dx - p.edgeDis, p.dy + p.edgeDis, p.dx + p.edgeDis,
              p.dy - p.edgeDis);
}

// 弹出胜利的消息框
int win(bool flag) {
    HWND win = GetHWnd();
    if (flag)
        return MessageBox(win, _T("黑色赢了"), _T("游戏结束！"),
                          MB_RETRYCANCEL);
    else
        return MessageBox(win, _T("白色赢了"), _T("游戏结束！"),
                          MB_RETRYCANCEL);
}

// 这一部分是对下棋的控制,属于键盘的操纵界面
point getClick(int flag) {
    point click1, click2;
    ExMessage m;
loop1:
    m = getmessage(EX_MOUSE);
    click1.dx = 0, click1.dy = 0;
    if (m.message != WM_LBUTTONDOWN)
        goto loop1;
    click1.dx = m.x, click1.dy = m.y;
    // 若第一次点击成功命中某个坐标
    if (click1.initiation()) {
    loop3:
        printEdge(click1, flag);
    loop2:
        m = getmessage(EX_MOUSE);
        click2.dx = 0, click2.dy = 0;
        if (m.message != WM_LBUTTONDOWN)
            goto loop2;
        click2.dx = m.x, click2.dy = m.y;
        if (click2.initiation()) {
            if (click2.x == click1.x && click2.y == click1.y)
                return click1;
            click1.x = click2.x, click1.y = click2.y;
            click2.x = 0, click2.y = 0;
            goto loop3;
        } else
            goto loop2;
    } else
        goto loop1;
}

// 这一部分是游戏的流程控制
int gameStart() {
    restart();
    bool flag = 0;
    while (true) {
        point a = getClick(flag);
        core.modify(a.x, a.y, flag + 1);
        print();
        flag = !flag;
        int f = core.judge();
        if (f == 1) {
            printf("White Win\n");
            return win(0);
        } else if (f == 2) {
            printf("Black Win\n");
            return win(1);
        }
    }
}
/*
    想法:数组一大块
    绘制棋盘
    判断胜负
*/