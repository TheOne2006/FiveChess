#include<point.h>
#include <game.h>
#include<iostream>
int main() {
    initiation();
	while (true) {
		if (gameStart() == IDCANCEL) break;
	}
	closegraph();			// 关闭绘图窗口
	return 0;
}