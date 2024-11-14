#include <myIO.h>
#include <fstream>
#include <game.h>
#include <iostream>
#include <string>

bool writeToF(myBoard::Board *b, std::string fileN) {
    std::fstream file;
    file.open(fileN, std::ios::out);
    if(!file) return false;
    for(int i = 0; i < b->numPieces; i++) 
        file << b->undoState[i] << std::endl;
    file.close();
}

std::vector<int> readFromF(std::string fileN) {
    std::fstream file;
    std::vector<int> ans;
    file.open(fileN, std::ios::in);
    int action;
    while(file >> action) 
        ans.push_back(action);
    file.close();
    return ans;
}