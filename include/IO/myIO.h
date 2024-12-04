#ifndef IO_H
#define IO_H
#include <game.h>
namespace my_io {
// 写入文件，文件名为当前时间
bool writeToF(myBoard::Board *b, int kind);
// 读入文件
std::vector<int> readFromF(std::string fileN);
// 初始化
void initiation();
// 修改存储的Dir
void modifyDir(std::string s);

std::string SelectFolder();

std::string SelectFile();
} // namespace my_io

#endif