#include <graphics.h>		
#include <conio.h>
#include <iostream>
#include <board.h>

// 这一部分是为了在棋盘核心和图像间建立坐标联系
struct point {
	const int startX = 22, startY = 22, dis = 35, R = 8, edgeDis = 10;
	int x = 0, y = 0;
	int dx, dy;

    // 构造函数，根据坐标确定棋盘上的位置
	bool initiation();
};

// 这一部分是对显示界面的控制
// 重新根据core绘制整个屏幕
void print();

// 对界面的一个初始化
void initiation();

// 打印第一次点击的框框
void printEdge(point p, int flag);

// 弹出胜利的消息框
int win(bool flag);

// 这一部分是对下棋的控制,属于键盘的操纵界面
point getClick(int flag);

// 这一部分是游戏的流程控制
int gameStart();