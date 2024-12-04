#ifndef GAME_NOW
#define GAME_NOW
#include <judge.h>
#include <point.h>

// 这一部分是对显示界面的控制
// 重新根据core绘制整个屏幕
// 一些公共变量
void print();

// 对界面的一个初始化
void initiation();

// 打印第一次点击的框框
void printEdge(myBoard::point p, int flag);

// 弹出胜利的消息框
int win(bool flag);

// 这一部分是对下棋的控制,属于键盘的操纵界面
myBoard::point getClick(int flag);

// 这一部分是游戏的流程控制
int gameSingleStart();

// 这一部分是游戏的流程控制
int gameTwoStart();

// 这一部分是游戏的流程控制
int gameLoadStart(std::vector<int> steps);

// 撤回一步的操作
bool undo();

// 往后一步操作
bool nextDo();

// 修改深度\获取深度
bool modifyDepth(int d);
int getDepth();

// 修改ai颜色\获取ai颜色
int getAIColor();
void modifyAi(int a);

// 给出棋盘信息
myBoard::Board *getCore();
int getKind();

#endif