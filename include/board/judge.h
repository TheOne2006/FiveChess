#ifndef JUDGE_H
#define JUDGE_H
#include <string>
#include <vector>

namespace myBoard {
class Board {
  private:
    const int boardLen = 15;

  public:
    const int EMPTY = -1;
    const int MY_BLACK = 1;
    const int MY_WHITE = 0;
    const int notOver = 2;
    const int flatFlag = 3;
    unsigned int numPieces = 0; // 现在已经下了几颗棋子
    int undoState[225];         // 撤回的放置步
    int currentPlayer = MY_BLACK;
    int preAction = -1;
    std::vector<int> state;
    std::vector<int> availableAction;
    int board[15][15] = {EMPTY};
    // 初始化函数
    Board();
    // 根据特定棋盘初始化函数
    Board(const Board *board);
    // 重新启动棋盘
    void reset();
    // 判断是否在棋盘内
    bool inBoard(int x, int y);
    // 获取未被填补的空缺
    std::vector<int> getAvailable();
    // 在此处下一个棋子
    void doAction(int action);
    // 撤回上一步下棋(两步)
    void undo();
    // 撤回一步下棋
    void undoOne();
    int getNumPipces();
    // 游戏是否结束
    int isGameOver();
    // 是否有胜者
    int win_end();
    // 判断是否长连
    bool long_connect();
    // 特殊三三判断
    bool tt_special_case(std::string &m_str, size_t pos, int t_case);
    // 特殊四四判断
    bool ff_special_case(std::string &m_str, size_t pos, int f_case);
    // 三三判断
    bool three_three();
    // 四四判断
    bool four_four();
    // 是否已经完成五连
    bool five_connect();

    // 这一部分是现在board.cpp中
    // get 函数
    int get(int x, int y);
    // modify 函数
    void modify(int x, int y);
    // 赋值函数
    void operator=(const myBoard::Board &B);
};
} // namespace myBoard
#endif