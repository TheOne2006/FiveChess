#include <judge.h>
#include <string>
#include <value.h>
#include <vector>
#define INF 10000000;

valuePattern vsSelf[20] = {
    // 活二
    {"011000", 20},
    {"010100", 10},
    {"001100", 26},
    {"001010", 10},
    {"000110", 20},
    // 死三
    {"o11100", 540},
    {"00111o", 540},
    {"o10110", 360},
    {"o11010", 360},
    {"01011o", 360},
    {"01101o", 360},
    // 活三
    {"001110", 780},
    {"011100", 780},
    {"011010", 540},
    {"010110", 540},
    // 活四
    {"01111", 100000},
    {"11110", 100000},
    {"11011", 100000},
    {"11101", 100000},
    {"10111", 100000}};

valuePattern vsYou[21] = {
    // 活二
    {"0oo000", 4},
    {"0o0o00", 2},
    {"00oo00", 5},
    {"00o0o0", 2},
    {"000oo0", 4},
    // 死三
    {"1ooo00", 180},
    {"00ooo1", 180},
    {"1o0oo0", 120},
    {"1oo0o0", 120},
    {"0o0oo1", 120},
    {"0oo0o1", 320},
    // 活三
    {"00ooo0", 260},
    {"0ooo00", 260},
    {"0oo0o0", 180},
    {"0o0oo0", 180},
    // 死四
    {"oo0oo", 6000},
    {"oooo0", 6000},
    {"0oooo", 6000},
    {"ooo0o", 6000},
    {"o0ooo", 6000},
    // 活四
    {"0oooo0", 50000},
};

std::vector<valuePattern> hasDone;

// 基本评分思路：针对白棋的评分：计算活三活四
// 当前是nflag下，对nflag的人进行计分
int getValue(myBoard::Board *b, int nflag) {
    int isWin = b->win_end();
    // 判断能胜利便直接返回
    if (isWin != b->notOver) {
        if (isWin == nflag) {
            return INF;
        } else {
            return -INF;
        }
    }
    std::string boardS;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (b->board[i][j] == nflag)
                boardS.append("1");
            else if (b->board[i][j] == -1)
                boardS.append("0");
            else
                boardS.append("o");
        }
    }
    for (int i = 0; i < hasDone.size(); i++) {
        if (hasDone[i].s == boardS)
            return hasDone[i].v;
    }
    int value = 0;
    // 横向
    std::string m;
    for (int i = 0; i < 15; i++) {
        m.clear();
        for (int j = 0; j < 15; j++) {
            if (b->board[i][j] == nflag)
                m.append("1");
            else if (b->board[i][j] == -1)
                m.append("0");
            else
                m.append("o");
        }
        for (int k = 0; k < 20; k++) {
            if (m.find(vsSelf[k].s) != std::string::npos)
                value += vsSelf[k].v;
        }
        for (int k = 0; k < 20; k++) {
            if (m.find(vsYou[k].s) != std::string::npos)
                value -= vsYou[k].v;
        }
    }
    // 纵向
    for (int j = 0; j < 15; j++) {
        m.clear();
        for (int i = 0; i < 15; i++) {
            if (b->board[i][j] == nflag)
                m.append("1");
            else if (b->board[i][j] == -1)
                m.append("0");
            else
                m.append("o");
        }
        for (int k = 0; k < 20; k++) {
            if (m.find(vsSelf[k].s) != std::string::npos)
                value += vsSelf[k].v;
        }
        for (int k = 0; k < 20; k++) {
            if (m.find(vsYou[k].s) != std::string::npos)
                value -= vsYou[k].v;
        }
    }
    // 斜向
    for (int p = -14; p < 15; p++) {
        m.clear();
        for (int i = std::max(0, -p), j = std::max(0, p); i < 15 && j < 15;
             i++, j++) {
            if (b->board[i][j] == nflag)
                m.append("1");
            else if (b->board[i][j] == -1)
                m.append("0");
            else
                m.append("o");
        }
        // 开始查询评分
        for (int k = 0; k < 20; k++) {
            if (m.find(vsSelf[k].s) != std::string::npos)
                value += vsSelf[k].v;
        }
        for (int k = 0; k < 20; k++) {
            if (m.find(vsYou[k].s) != std::string::npos)
                value -= vsYou[k].v;
        }
    }
    // 斜向
    for (int p = 0; p < 28; p++) {
        m.clear();
        for (int i = std::min(14, p), j = std::max(0, p - 14); i >= 0 && j < 15;
             i--, j++) {
            if (b->board[i][j] == nflag)
                m.append("1");
            else if (b->board[i][j] == -1)
                m.append("0");
            else
                m.append("o");
        }
        // 开始查询评分
        for (int k = 0; k < 20; k++) {
            if (m.find(vsSelf[k].s) != std::string::npos)
                value += vsSelf[k].v;
        }
        for (int k = 0; k < 20; k++) {
            if (m.find(vsYou[k].s) != std::string::npos)
                value -= vsYou[k].v;
        }
    }
    hasDone.push_back({boardS, value});
    // // test
    // if (value > 6000) {
    //     printf("nflag:%d ---\n", nflag);
    //     for (int j = 0; j < 15; j++) {
    //         for (int i = 0; i < 15; i++) {
    //             if (b->board[i][j] == -1) {
    //                 printf(". ");
    //             } else if (b->board[i][j] == nflag) {
    //                 printf("o ");
    //             } else {
    //                 printf("* ");
    //             }
    //         }
    //         printf("\n");
    //     }
    //     printf("value:%d\n-------------\n", value);
    // }

    return value;
}