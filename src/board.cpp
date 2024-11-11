// 这一部分是维护整个棋盘的内部核心数据
#include "board.h"
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

bool board::modify(int x, int y, int k) {
    if (board[x][y])
        return false;
    else {
        board[x][y] = k, now++, p = now;
        for(int i = 0; i < 15; i++) 
            for(int j = 0; j < 15; j++)
                allBoard[p][i][j] = board[x][y];
    }
    return true;
}

int board::get(int x, int y) {
    return board[x][y];
}

// 返回当前在的步数
int board::getNowBoard() {
    return now;
}

int board::getMaxBoard() {
    return p;
}

// 将现有的棋盘更改为某一步棋盘
void board::modifyNowBoard(int h) {
    now = h;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++) 
            board[i][j] = allBoard[now][i][j];
}
