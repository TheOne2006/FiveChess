#include <AI.h>

// 内部的演算棋盘，第一步为棋盘现在的状态
int treeBoard[100][16][16]; // 1为玩家 2为人机

// 调节AI的“聪明”程度, 即搜索深度
int K = 3, INF = 1000000;

int dire[8][2] = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                  {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

// 简单的剪枝（因为一般不会把棋下在离所有棋子4格远的地方）
bool isNear(int x, int y, int p) {
    if (treeBoard[p][x][y] > 0)
        return false;
    bool flag = false;
    for (int k = 0; k < 8 && !flag; k++) {
        int nx = x + dire[k][0], ny = y + dire[k][1];
        for (int m = 1;
             m <= 4 && !flag && (nx >= 1 && nx <= 15) && (ny >= 1 && ny <= 15);
             m++) {
            flag |= treeBoard[p][nx][ny];
            nx += dire[k][0], ny += dire[k][1];
        }
    }
    return flag;
}

// 判断此时能否下在这里的isOk函数（即ai下在这里是否为必输）

// 价值函数对当前图像进行评分 此时必为AI所下
struct value {
    int v, a[6];
};
// 1代表己方棋子，0代表空地，-1代表不考虑这枚棋子
value vs[10] = {
    {INF, {1, 1, 1, 1, 1, -1}},    // 赢了
    {INF / 4, {0, 1, 1, 1, 1, 0}}, // 已经出现两头空的4

    {4320, {-1, 1, 1, 1, 1, 0}}, // 一头被堵的4
    {4320, {1, 1, 0, 1, 1, -1}}, // 还是一个能连成5
    {4320, {1, 1, 0, 1, 1, -1}}, // 还是一个能连成5
    {4320, {1, 1, 1, 0, 1, -1}}, // 还是一个能连成5

    {720, {0, 1, 1, 0, 1, 0}}, // 相当于1个3
    {720, {0, 1, 1, 1, 0, 0}}, // 还是1个活 两头可以下的活三

    {120, {0, 0, 1, 0, 1, 0}},  // 可以发展的2
    {120, {0, 1, 1, 0, 0, -1}}, // 可以发展的2
};

int getPointValue(int i, int j, int k, int p) {
    int l = 0, ans = 0;
    while (l < 10) {
        for (int k = 0; k < 8; k++) {
            int flag = 1, nx = i, ny = j;
            for (int m = 0; m < 6 && flag; m++) {
                bool isIn = nx <= 1 || nx >= 15 || ny <= 1 || ny >= 15,
                     isOk = (treeBoard[p][nx][ny] == -1) |
                            (treeBoard[p][nx][ny] == 0 && vs[l].a[m] == 0) |
                            (treeBoard[p][nx][ny] == k && vs[l].a[m] == 1);
                if (!(isIn && isOk))
                    flag = false;
                nx += dire[k][0], ny += dire[k][1];
            }
            if (flag)
                ans -= vs[l].v;
        }
        l++;
    }
}

int getValue(int p) {
    int ans = 0;
    // 判断对手和己方的棋局
    for (int i = 1; i <= 15; i++) {
        for (int j = 1; j <= 15; j++) {
            ans += getPointValue(i, j, 1, p) - getPointValue(i, j, 2, p);
        }
    }
    return ans;
}

struct ab {
    int top, bottom;
};

// 深度搜索函数来判定, p为1时是min节点, p为0时是max节点
int dfs(int now, int p, ab nw) {
    if (now == 2 * K + 1)
        return getValue(now);
    for (int k = 1; k <= 15; k++)
        for (int m = 1; m <= 15; m++)
            treeBoard[now + 1][k][m] = treeBoard[now][k][m];

    for (int i = 1; i <= 15; i++) {
        for (int j = 1; j <= 15; j++) {
            if (!isNear(i, j, now))
                continue;
            // 中间要加入许多关键的价值判断才行
            if (p) {
                treeBoard[now + 1][i][j] = 1;
                int k = dfs(now + 1, 0, nw);
                if (k <= nw.bottom)
                    return -INF;
                nw.top = std::min(k, nw.top);
                treeBoard[now + 1][i][j] = treeBoard[now][i][j];
            } else {
                treeBoard[now + 1][i][j] = 2;
                int k = dfs(now + 1, 1, nw);
                if (k >= nw.top)
                    return INF;
                nw.bottom = std::max(k, nw.bottom);
                treeBoard[now + 1][i][j] = treeBoard[now][i][j];
            }
        }
    }
    if (p)
        return nw.top;
    else
        return nw.bottom;
}

// 返回下一步应该走的位置
point moveNext(board *b) {
    int pa, pb;
    for (int i = 1; i <= 15; i++) {
        for (int j = 1; j <= 15; j++) {
            treeBoard[2][i][j] = treeBoard[1][i][j] = (*b).get(i, j);
        }
    }
    for (int i = 1; i <= 15; i++)
        for (int j = 1; j <= 15; j++) {
            if (!isNear(i, j, 1))
                continue;
            ab ans = {INF, -INF};
            treeBoard[2][i][j] = 2;
            int k = dfs(2, 1, ans);
            if (ans.bottom < k) {
                ans.bottom = k, pa = i, pb = j;
            }
            treeBoard[2][i][j] = 0;
        }
    point ans;
    ans.x = pa, ans.y = pb, ans.initiation();
    return ans;
}