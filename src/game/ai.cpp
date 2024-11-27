#include <ai.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <value.h>
#include <vector>
#define INF 10000000
using namespace ai;
// 一个会变动的棋盘
myBoard::Board bb;
int de;
// WHITE 0, BLACK 1 EMPTY -1

AI::AI(int f, int d) {
    core = new myBoard::Board();
    flag = f, depth = d;
}

AI::AI(myBoard::Board *a, int f, int d) {
    core = new myBoard::Board(a);
    flag = f, depth = d;
}

AI::~AI() { delete core; }

void AI::doAction(int action) { core->doAction(action); }

void AI::undo() { core->undo(); }

void AI::modify(int x, int y) { core->modify(x, y); }

void AI::modifyDepth(int d) { depth = d; }

int getValue(myBoard::Board *b, int nflag);

bool isIn(int x, int y);

bool isNear(int x, int y);

int getNext(int nflag, int d, int alpha, int beta);

struct Situation {
    myBoard::point t;
    int p;
};

valuePattern liveFour[100];

bool judegeLiveFour(int i, int j, int flag) {
    std::string four = "011110", m;
    // 横线
    for (int p = i - 4; p <= i - 1; p++) {
        m.clear();
        for (int k = 0; k <= 5; k++) {
            if (!isIn(p + k, j))
                break;
            if (bb.board[p + k][j] == flag)
                m.append("1");
            else if (bb.board[p + k][j] == -1)
                m.append("0");
            else
                m.append("o");
        }
        if (m == four)
            return true;
    }

    // 竖线
    for (int p = j - 4; p <= j - 1; p++) {
        m.clear();
        for (int k = 0; k <= 5; k++) {
            if (!isIn(i, p + k))
                break;
            if (bb.board[i][p + k] == flag)
                m.append("1");
            else if (bb.board[i][p + k] == -1)
                m.append("0");
            else
                m.append("o");
        }
        if (m == four)
            return true;
    }

    // 斜线
    for (int p2 = j - 4, p1 = i - 4; 2 <= j - 1 && p1 <= i - 1; p1++, p2++) {
        m.clear();
        for (int k = 0; k <= 5; k++) {
            if (!isIn(p1 + k, p2 + k))
                break;
            if (bb.board[p1 + k][p2 + k] == flag)
                m.append("1");
            else if (bb.board[p1 + k][p2 + k] == -1)
                m.append("0");
            else
                m.append("o");
        }
        if (m == four)
            return true;
    }

    // 斜线
    for (int p2 = j - 4, p1 = i + 4; p2 <= j - 1 && p1 >= i + 1; p1--, p2++) {
        m.clear();
        for (int k = 0; k <= 5; k++) {
            if (!isIn(p1 - k, p2 + k))
                break;
            if (bb.board[p1 - k][p2 + k] == flag)
                m.append("1");
            else if (bb.board[p1 - k][p2 + k] == -1)
                m.append("0");
            else
                m.append("o");
        }
        if (m == four)
            return true;
    }

    return false;
}

std::vector<Situation> getAvailable(int flag) {
    std::vector<Situation> readyPut;
    int permis = 0;
    // 考虑下了之后直接胜利的点
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (bb.board[i][j] == -1 && isNear(i, j)) {
                bb.modify(i, j);
                int isWin = bb.win_end();
                if (isWin == flag) {
                    myBoard::point p;
                    p.initiationWithXY(i, j);
                    readyPut.push_back({p, 0});
                    permis = 4;
                }
                bb.undoOne();
            }
        }
    }

    // 考虑没下了之后如果必输的点
    // 先直接修改currentPlayer为对方
    if (permis < 4) {
        int px, py;
        bool fa = true;
        for (px = 0; px < 15 && fa; px++)
            for (py = 0; py < 15 && fa; py++)
                fa = isNear(px, py);
        bb.modify(px, py);

        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if (bb.board[i][j] == -1 && isNear(i, j)) {
                    bb.modify(i, j);
                    int isWin = bb.win_end();
                    if (isWin == 1 - flag) {
                        myBoard::point p;
                        p.initiationWithXY(i, j);
                        readyPut.push_back({p, 0});
                        permis = 3;
                    }
                    bb.undoOne();
                }
            }
        }
        // 再修改回来
        bb.undoOne();
    }

    // 考虑下了之后自己成活四的点
    if (permis < 3) {
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if (bb.board[i][j] == -1 && isNear(i, j)) {
                    bb.modify(i, j);
                    int isWin = bb.win_end();
                    if (isWin == bb.notOver) {
                        if (judegeLiveFour(i, j, flag)) {
                            myBoard::point p;
                            p.initiationWithXY(i, j);
                            readyPut.push_back({p, 0});
                            permis = 2;
                        }
                    }
                    bb.undoOne();
                }
            }
        }
    }

    // 考虑对方下了成活四的点
    if (permis < 2) {
        int px, py;
        bool fa = true;
        for (px = 0; px < 15 && fa; px++)
            for (py = 0; py < 15 && fa; py++)
                fa = isNear(px, py);
        bb.modify(px, py);
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if (bb.board[i][j] == -1 && isNear(i, j)) {
                    bb.modify(i, j);
                    int isWin = bb.win_end();
                    if (isWin == bb.notOver) {
                        if (judegeLiveFour(i, j, flag)) {
                            myBoard::point p;
                            p.initiationWithXY(i, j);
                            readyPut.push_back({p, 0});
                            permis = 1;
                        }
                    }
                    bb.undoOne();
                }
            }
        }
        // 再修改回来
        bb.undoOne();
    }

    // 接下来什么都不考虑了
    if (permis < 1) {
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if (bb.board[i][j] == -1 && isNear(i, j)) {
                    myBoard::point p;
                    p.initiationWithXY(i, j);
                    readyPut.push_back({p, 0});
                }
            }
        }
    }

    // 最终给个跑分
    for (int i = 0; i < readyPut.size(); i++) {
        myBoard::point p = readyPut[i].t;
        bb.modify(p.x, p.y);
        readyPut[i].p = getValue(&bb, flag);
        bb.undoOne();
    }
    return readyPut;
}

bool compare(const Situation &s1, const Situation &s2) { return s1.p > s2.p; }

int kq = 0;
myBoard::point AI::getNextStep() {
    kq++;
    bb = *core, de = this->depth;
    std::vector<Situation> readyPut;
    if (bb.numPieces) {
        // readyPut = getAvailable(flag);
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if (bb.board[i][j] == -1 && isNear(i, j)) {
                    myBoard::point p;
                    p.initiationWithXY(i, j);
                    bb.doAction(p.p);
                    readyPut.push_back({p, getValue(&bb, flag)});
                    bb.undoOne();
                }
            }
        }
    } else {
        myBoard::point p;
        p.initiationWithXY(7, 7), bb.modify(7, 7);
        int score = getValue(&bb, 1 - flag);
        readyPut.push_back({p, score});
        bb.undoOne();
    }
    // printf("initiate:%d\n", kq);
    // for (int j = 0; j < 15; j++) {
    //     for (int i = 0; i < 15; i++) {
    //         if (core->board[i][j] == -1) {
    //             printf(". ");
    //         } else if (core->board[i][j] == 0) {
    //             printf("o ");
    //         } else {
    //             printf("* ");
    //         }
    //     }
    //     printf("\n");
    // }
    // 进行搜索，先进行简单剪枝
    std::sort(readyPut.begin(), readyPut.end(), compare);
    myBoard::point now;
    std::vector<myBoard::point> ans;
    int score = -INF;
    int si = readyPut.size();
    while (readyPut.size() > 0) {
        now = readyPut[readyPut.size() - 1].t, readyPut.pop_back();
        bb.doAction(now.p);
        int isWin = bb.win_end();
        // 判断能胜利便直接返回
        if (isWin != bb.notOver) {
            if (isWin == flag) {
                bb.undoOne();
                return now;
            } else {
                // 如果输了就不进入下一次
                bb.undoOne();
                continue;
            }
        }
        // 以下了一步棋后的所有局面的最小值的相反数作为该局面的最大值
        // bottom 表示这颗节点的母节点上的另外一个分支得到的值
        int t = -getNext(1 - flag, 1, -INF, -score);
        if (score < t) {
            score = t, ans = std::vector<myBoard::point>();
            ans.push_back(now);
        } else if (score == t) {
            ans.push_back(now);
        }
        bb.undoOne();
    }
    int ra = rand();
    return ans[ra % ans.size()];
}

// 表示四个方向
int dire[16][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

bool isIn(int x, int y) { return (x >= 0) && (x < 15) && (y >= 0) && (y < 15); }

bool isNear(int x, int y) {
    bool re = false;
    for (int i = x - 1; i <= x + 1 && !re; i++) {
        for (int j = y - 1; j <= y + 1 && !re; j++) {
            if (!isIn(i, j))
                continue;
            if (bb.board[i][j] != -1) {
                re = true;
                break;
            }
        }
    }
    return re;
}

// 这个局面下nflag下棋能获得的最大分数(对nflag而言), depth 记录搜索深度
// Rap 表示搜索树上的下限，利用alpha-beta剪枝
// 剪枝第二个策略：搜索时先对节点给个估值
int getNext(int nflag, int d, int alpha, int beta) {
    if (d == de)
        return getValue(&bb, nflag);
    std::vector<Situation> readyPut;
    // readyPut = getAvailable(nflag);
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (bb.board[i][j] == -1 && isNear(i, j)) {
                myBoard::point p;
                p.initiationWithXY(i, j);
                bb.doAction(p.p);
                readyPut.push_back({p, getValue(&bb, nflag)});
                bb.undoOne();
            }
        }
    }
    std::sort(readyPut.begin(), readyPut.end(), compare);
    // 进行搜索，先进行简单剪枝
    myBoard::point now;
    int score = -INF;
    int si = readyPut.size();
    while (readyPut.size() > 0) {
        now = readyPut[readyPut.size() - 1].t, readyPut.pop_back();
        bb.doAction(now.p);
        int isWin = bb.win_end();
        // 判断能胜利便直接返回
        if (isWin != bb.notOver) {
            if (isWin == nflag) {
                bb.undoOne();
                return INF;
            } else {
                // 如果输了就不进入下一次
                bb.undoOne();
                continue;
            }
        }
        // 以下了一步棋后的所有局面的最小值的相反数作为该局面的最大值
        // bottom 表示这颗节点的母节点上的另外一个分支得到的值
        int t = -getNext(1 - nflag, d + 1, -beta, -alpha);
        score = std::max(t, score);
        alpha = std::max(alpha, score);
        bb.undoOne();
        if (alpha >= beta) {
            return score;
        }
    }
    // //会被考察的点
    // //test
    // printf("nflag:%d d:%d score:%d :\n", nflag, d, score);
    // for (int j = 0; j < 15; j++) {
    //     for (int i = 0; i < 15; i++) {
    //         if (bb.board[i][j] == -1) {
    //             printf(". ");
    //         } else if (bb.board[i][j] == 0) {
    //             printf("o ");
    //         } else {
    //             printf("* ");
    //         }
    //     }
    //     printf("\n");
    // }
    // printf("\n-------------\n");
    return score;
}