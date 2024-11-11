#include <AI.h>

using namespace AI;

// 内部的演算棋盘，第一步为棋盘现在的状态
int treeBoard[100][16][16]; // 1为白色 2为黑色（黑色即AI所选色）

// 调节AI的“聪明”程度, 即搜索深度
int K = 10, INF = 1000000;

int dire[8][2] = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                  {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

// 价值函数对当前图像进行评分
int getValue(int p) {
    
}

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
            nx += dire[k][0], ny += dire[k][1];
            flag |= treeBoard[p][nx][ny];
        }
    }
    return flag;
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
point move(board *b) {
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
            if(ans.bottom < k) {
                ans.bottom = k, pa = i, pb = j;
            }
            treeBoard[2][i][j] = 0;
        }
    point ans;
    ans.x = pa, ans.y = pb, ans.initiation();
    return ans;
}