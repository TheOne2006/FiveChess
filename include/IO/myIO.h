#ifndef IO_H
#define IO_H
#include <game.h>

bool writeToF(myBoard::Board *b, std::string fileN);

std::vector<int> readFromF(std::string fileN);

#endif