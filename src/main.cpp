#include <show.h>

int main() {
    initiation();
	while (true) {
		if (gameStart() == IDCANCEL) break;
	}
	closegraph();			// 关闭绘图窗口
	return 0;
}