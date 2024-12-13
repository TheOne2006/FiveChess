#ifndef AI_H
#define AI_H
#define WHITE_COLOR 0
#define BLACK_COLOR 1
#include <judge.h>
#include <point.h>
namespace ai {
class AI {
    private:
        myBoard::Board *core;
        int flag, depth;
    public:
        // f表示当前AI以什么身份下棋，外部维护核心棋盘的正确，depth 表示搜索深度
        AI(myBoard::Board * a, int f, int d);
        AI(int f, int d);
        ~AI();
        // 在此处下一个棋子
        void doAction(int action);
        // 在x,y下一个棋子
        void modify(int x, int y);
        // 撤回上一步下棋
        void undo();
        // 下一步棋子
        myBoard::point getNextStep();
        // 修改搜索深度
        void modifyDepth(int d);
};
}; // namespace ai
// 设计思路：创建一个AI的player
// 把getClick和其他的库分开，并且判断黑白手，直接写死即可

#endif