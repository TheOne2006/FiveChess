#include <judge.h>
#include <point.h>
using namespace myBoard;
// get 函数
int Board::get(int x, int y) { return board[x][y]; }
// modify 函数
void Board::modify(int x, int y) {
    point a;
    a.initiationWithXY(x, y), doAction(a.p);
}