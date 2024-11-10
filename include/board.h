// 这一部分是维护整个棋盘的内部核心数据
struct board {
	private: int board[16][16];
	public:
	// 判断某两个点的颜色是否相同
	bool judge_dire(int x, int y, int i, int j);

	// 判断某个点是否为胜子
	bool judge_point(int x, int y);

    // 判断整个棋盘是否有胜者
	int judge();

    // 修改某个点的颜色
	int modify(int x, int y, int k);

	// 返回某个点的颜色
	int get(int x, int y);
};