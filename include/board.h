// 这一部分是维护整个棋盘的内部核心数据
struct board {
	private: 
		// 现有的board信息(根据此来修改点)
		int board[16][16];

		// 所有的历史棋盘信息
		int allBoard[250][16][16];
		// p是历史最大的棋盘数
		int p = 1;
		// now是现在的棋盘数
		int now = 1;

	public:
	// 判断某两个点的颜色是否相同
	bool judge_dire(int x, int y, int i, int j);

	// 判断某个点是否为胜子
	bool judge_point(int x, int y);

    // 判断整个棋盘是否有胜者
	int judge();

    // 修改某个点的颜色
	bool modify(int x, int y, int k);

	// 返回某个点的颜色
	int get(int x, int y);

	// 返回当前在的步数
	int getNowBoard();

	// 将现有的棋盘更改为某一步棋盘
	void modifyNowBoard(int p);

	// 返回历史最大数
	int getMaxBoard();

	// 赋予初始棋盘
	bool initiate(int a[16][16]);
};

// 这一部分是为了在棋盘核心和图像间建立坐标联系
struct point {
	const int startX = 22, startY = 22, dis = 35, R = 8, edgeDis = 10;
	int x = 0, y = 0;
	int dx, dy;

    // 构造函数，根据坐标确定棋盘上的位置
	bool initiation();
};
