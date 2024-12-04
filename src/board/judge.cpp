// 标准裁判库
#include "judge.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <sstream>
using namespace std;

using namespace myBoard;

void Board::operator=(const myBoard::Board &B) {
    numPieces = B.numPieces;
    currentPlayer = B.currentPlayer;
    availableAction.clear();
    availableAction = B.availableAction;
    for (size_t i = 0; i < 225; i++) {
        undoState[i] = B.undoState[i];
    }
    for (size_t i = 0; i < 15; i++) {
        for (size_t j = 0; j < 15; j++) {
            board[i][j] = B.board[i][j];
        }
    }
    state = B.state;
}

Board::Board() { reset(); }

Board::Board(const Board *b) {
    numPieces = b->numPieces;
    currentPlayer = b->currentPlayer;
    availableAction.clear();
    availableAction = b->availableAction;
    for (size_t i = 0; i < 225; i++) {
        undoState[i] = b->undoState[i];
    }
    for (size_t i = 0; i < 15; i++) {
        for (size_t j = 0; j < 15; j++) {
            board[i][j] = b->board[i][j];
        }
    }
    state = b->state;
}

void Board::reset() {
    state.clear();
    state.insert(state.begin(), 255, -1);
    numPieces = 0;
    currentPlayer = MY_BLACK;
    availableAction.clear();
    for (size_t i = 0; i < 225; i++) {
        this->availableAction.push_back(i);
    }
    memset(undoState, -1, sizeof(undoState));
    memset(board, EMPTY, sizeof(board));
}

bool Board::inBoard(int x, int y) {
    return x >= 0 && x < boardLen && y >= 0 && y < boardLen;
}

int Board::getNumPipces() { return numPieces; }

std::vector<int> Board::getAvailable() {
    std::vector<int> available;
    for (size_t i = 0; i < boardLen; i++) {
        for (size_t j = 0; j < boardLen; j++) {
            if (board[i][j] == EMPTY) {
                available.push_back(i * boardLen + j);
            }
        }
    }
    return available;
}

void Board::doAction(int action) {
    board[int(action / boardLen)][action % boardLen] = currentPlayer;
    preAction = action;
    state[action] = currentPlayer;
    undoState[numPieces] = action;
    numPieces++;
    currentPlayer = MY_BLACK + MY_WHITE - currentPlayer;
    availableAction.clear();
    availableAction = getAvailable();
}

// 撤回一次同时撤回2步
void Board::undo() {
    // 如果只有2个
    if(numPieces == 2) {
        reset();
        return;
    }
    // 1
    board[int(preAction / boardLen)][int(preAction % boardLen)] = EMPTY;
    state[preAction] = -1;
    undoState[numPieces - 1] = -1;
    numPieces--;
    preAction = undoState[numPieces - 1];
    // 2
    board[int(preAction / boardLen)][int(preAction % boardLen)] = EMPTY;
    state[preAction] = -1;
    undoState[numPieces - 1] = -1;
    numPieces--;
    preAction = undoState[numPieces - 1];
    availableAction.clear();
    availableAction = getAvailable();
}

void Board::undoOne() {
    if(numPieces == 1) {
        reset();
        return;
    }
    // 1
    board[int(preAction / boardLen)][int(preAction % boardLen)] = EMPTY;
    state[preAction] = -1;
    undoState[numPieces - 1] = -1;
    numPieces--;
    preAction = undoState[numPieces - 1];
    // 置换下棋手
    currentPlayer = MY_BLACK + MY_WHITE - currentPlayer;
    availableAction.clear();
    availableAction = getAvailable();
}

int Board::isGameOver() {
    if (numPieces > 5) {
        int we = this->win_end();
        if (we == notOver) {
            if (find(this->state.begin(), this->state.end(), -1) ==
                state.end()) {
                return flatFlag;
            }
            return notOver;
        }
        return we;
    }
    return notOver;
}

int Board::win_end() {
    int last_player = this->currentPlayer ^ 1;
    if (last_player == MY_BLACK) {
        if (long_connect())
            return MY_WHITE;
        if (five_connect())
            return MY_BLACK;
        if (three_three())
            return MY_WHITE;
        if (four_four())
            return MY_WHITE;
    } else {
        if (this->five_connect())
            return MY_WHITE;
    }
    return notOver;
}

bool Board::long_connect() {
    int h = this->preAction / 15;
    int w = this->preAction % 15;
    int last_move = this->preAction;
    int ret;

    int bias = std::min(w, 5);
    for (int i = last_move - bias; i < last_move + 1; i++) {
        if (15 - 1 - i % 15 < 5) {
            break;
        }
        ret = 0;
        for (int k = i; k < i + 6; k++) {
            if (this->state[k] != (this->currentPlayer ^ 1)) {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
            return true;
    }
    bias = std::min(h, 5);
    for (int i = last_move - bias * 15; i < last_move + 15;
         i += 15) {
        if (15 - 1 - i / 15 < 5) {
            break;
        }
        ret = 0;
        for (int k = i; k < i + 6 * 15; k += 15) {
            if (this->state[k] != (this->currentPlayer ^ 1)) {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
            return true;
    }
    bias = std::min(std::min(h, 5), std::min(w, 5));
    for (int i = last_move - bias * 15 - bias;
         i < last_move + 15 + 1; i += 15 + 1) {
        if ((15 - 1 - i / 15 < 5) ||
            (15 - 1 - i % 15 < 5)) {
            break;
        }
        ret = 0;
        for (int k = i; k < i + 6 * 15 + 6; k += 15 + 1) {
            if (this->state[k] != (this->currentPlayer ^ 1)) {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
            return true;
    }
    bias = std::min(std::min(15 - 1 - h, 5), std::min(w, 5));
    for (int i = last_move + bias * 15 - bias;
         i > last_move - 15 + 1; i += (-(15) + 1)) {
        if ((15 - 1 - i % 15 < 5) || (i / 15 < 5)) {
            break;
        }
        ret = 0;
        for (int k = i; k > i - 6 * 15 + 6; k += (-(15) + 1)) {
            if (this->state[k] != (this->currentPlayer ^ 1)) {
                ret = 1;
                break;
            }
        }
        if (ret == 0)
            return true;
    }
    return false;
}

bool Board::tt_special_case(std::string &m_str, size_t pos, int t_case) {
    if (t_case == 1) { // oo111o
        if (pos + 6 < m_str.size()) {
            if (m_str[pos + 6] == '1')
                return true;
        }
    } else { // o111oo
        if (pos > 0) {
            if (m_str[pos - 1] == '1')
                return true;
        }
    }
    return false;
}

bool Board::ff_special_case(std::string &m_str, size_t pos, int f_case) {
    if (f_case == 1) { // oo111o
        if (pos > 0) {
            if (m_str[pos - 1] == '1')
                return true;
        }
        if (pos + 5 < m_str.size()) {
            if (m_str[pos + 5] == '1')
                return true;
        }
        return false;
    } else if (f_case == 2) {
        if (pos > 0) {
            if (pos + 6 < m_str.size()) {
                if (m_str[pos - 1] == '1' &&
                    ((m_str[pos + 5] == 'o' && m_str[pos + 6] == '1') ||
                     (m_str[pos + 5] == '0')))
                    return true;
                return false;
            }
            if (pos + 5 < m_str.size()) {
                if (m_str[pos - 1] == '1' && m_str[pos + 5] == '0')
                    return true;
                return false;
            }
            if (m_str[pos - 1] == '1')
                return true;
            return false;
        } else
            return false;
    } else {
        if (pos + 5 < m_str.size()) {
            if (pos - 2 >= 0) {
                if ((m_str[pos - 2] == '1' && m_str[pos - 1] == 'o') ||
                    (m_str[pos - 1] == '0') && m_str[pos + 5] == '1')
                    return true;
                return false;
            } else if (pos - 1 >= 0) {
                if (m_str[pos + 5] == '1' && m_str[pos - 1] == '0')
                    return true;
                return false;
            }
            if (m_str[pos + 5] == '1')
                return true;
            return false;
        } else
            return false;
    }
}

bool Board::three_three() {
    // 获取坐标
    int h = this->preAction / 15;
    int w = this->preAction % 15;
    int last_move = this->preAction;
    // 棋盘宽度
    int width = 15;
    // 四种三三形式
    string jt1 = "o1o11o";
    string jt2 = "o11o1o";
    string ct1 = "oo111o";
    string ct2 = "o111oo";
    int three = 0;
    string m_str;
    size_t pos; // 和int没区别
    int bias = min(w, 4);
    for (int i = last_move - bias; i < last_move + min(width - 1 - w, 4) + 1;
         i++) {
        if (this->state[i] == -1) {
            m_str.append(1, 'o');
        } else if (this->state[i] == 0) {
            m_str.append(1, '0');
        } else {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(ct1);
    if (pos != m_str.npos) {
        if (this->tt_special_case(m_str, pos, 1) == false)
            three++;
    } else {
        pos = m_str.find(ct2);
        if (pos != m_str.npos) {
            if (this->tt_special_case(m_str, pos, 2) == false)
                three++;
        }
    }
    if (m_str.find(jt1) != m_str.npos)
        three++;
    if (m_str.find(jt2) != m_str.npos)
        three++;
    if (three > 1)
        return true;

    m_str.clear();
    bias = min(h, 4);
    for (int i = last_move - bias * width;
         i < last_move + width * min(width - 1 - h, 4) + width; i += width) {
        if (this->state[i] == -1) {
            m_str.append(1, 'o');
        } else if (this->state[i] == 0) {
            m_str.append(1, '0');
        } else {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(ct1);
    if (pos != m_str.npos) {
        if (this->tt_special_case(m_str, pos, 1) == false)
            three++;
    } else {
        pos = m_str.find(ct2);
        if (pos != m_str.npos) {
            if (this->tt_special_case(m_str, pos, 2) == false)
                three++;
        }
    }
    if (m_str.find(jt1) != m_str.npos)
        three++;
    if (m_str.find(jt2) != m_str.npos)
        three++;
    if (three > 1)
        return true;

    m_str.clear();
    bias = min(min(h, 4), min(w, 4));
    for (int i = last_move - bias * width - bias;
         i < last_move +
                 (width + 1) * min(min(width - 1 - h, width - 1 - w), 4) +
                 width + 1;
         i += width + 1) {
        if (this->state[i] == -1) {
            m_str.append(1, 'o');
        } else if (this->state[i] == 0) {
            m_str.append(1, '0');
        } else {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(ct1);
    if (pos != m_str.npos) {
        if (this->tt_special_case(m_str, pos, 1) == false)
            three++;
    } else {
        pos = m_str.find(ct2);
        if (pos != m_str.npos) {
            if (this->tt_special_case(m_str, pos, 2) == false)
                three++;
        }
    }
    if (m_str.find(jt1) != m_str.npos)
        three++;
    if (m_str.find(jt2) != m_str.npos)
        three++;
    if (three > 1)
        return true;

    m_str.clear();
    bias = min(min(width - 1 - w, 4), min(h, 4));
    for (int i = last_move - bias * (width - 1);
         i < last_move + (width - 1) * min(min(width - 1 - h, min(w, 4)), 4) +
                 width - 1;
         i += width - 1) {
        if (this->state[i] == -1) {
            m_str.append(1, 'o');
        } else if (this->state[i] == 0) {
            m_str.append(1, '0');
        } else {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(ct1);
    if (pos != m_str.npos) {
        if (this->tt_special_case(m_str, pos, 1) == false)
            three++;
    } else {
        pos = m_str.find(ct2);
        if (pos != m_str.npos) {
            if (this->tt_special_case(m_str, pos, 2) == false)
                three++;
        }
    }
    if (m_str.find(jt1) != m_str.npos)
        three++;
    if (m_str.find(jt2) != m_str.npos)
        three++;
    if (three > 1)
        return true;

    return false;
}

bool Board::four_four() {
    int h = this->preAction / 15;
    int w = this->preAction % 15;
    int last_player = this->currentPlayer ^ 1;
    int last_move = this->preAction;
    int width = 15;
    size_t pos;
    string jf1 = "111o1";
    string jf2 = "1o111";
    string jf3 = "11o11";
    string cf1 = "o1111";
    string cf2 = "1111o";
    int four = 0;
    string m_str;
    int bias;

    bias = min(w, 5);
    for (int i = last_move - bias; i < last_move + min(width - 1 - w, 5) + 1;
         i++) {
        if (this->state[i] == -1) {
            m_str.append(1, 'o');
        } else if (this->state[i] == 0) {
            m_str.append(1, '0');
        } else {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(jf1);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf2);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf3);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(cf1);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 2) == false)
            four++;
    } else {
        pos = m_str.find(cf2);
        if (pos != m_str.npos) {
            if (this->ff_special_case(m_str, pos, 3) == false)
                four++;
        }
    }
    if (four > 1)
        return true;

    m_str.clear();
    bias = min(h, 5);
    for (int i = last_move - bias * width;
         i < last_move + width * min(width - 1 - h, 5) + width; i += width) {
        if (this->state[i] == -1) {
            m_str.append(1, 'o');
        } else if (this->state[i] == 0) {
            m_str.append(1, '0');
        } else {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(jf1);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf2);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf3);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(cf1);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 2) == false)
            four++;
    } else {
        pos = m_str.find(cf2);
        if (pos != m_str.npos) {
            if (this->ff_special_case(m_str, pos, 3) == false)
                four++;
        }
    }
    if (four > 1)
        return true;

    m_str.clear();
    bias = min(min(h, 5), min(w, 5));
    for (int i = last_move - bias * width - bias;
         i < last_move +
                 (width + 1) * min(min(width - 1 - h, width - 1 - w), 5) +
                 width + 1;
         i += width + 1) {
        if (this->state[i] == -1) {
            m_str.append(1, 'o');
        } else if (this->state[i] == 0) {
            m_str.append(1, '0');
        } else {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(jf1);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf2);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf3);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(cf1);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 2) == false)
            four++;
    } else {
        pos = m_str.find(cf2);
        if (pos != m_str.npos) {
            if (this->ff_special_case(m_str, pos, 3) == false)
                four++;
        }
    }
    if (four > 1)
        return true;

    m_str.clear();
    bias = min(min(width - 1 - w, 5), min(h, 5));
    for (int i = last_move - bias * (width - 1);
         i < last_move + (width - 1) * min(min(width - 1 - h, min(w, 5)), 5) +
                 width - 1;
         i += width - 1) {
        if (this->state[i] == -1) {
            m_str.append(1, 'o');
        } else if (this->state[i] == 0) {
            m_str.append(1, '0');
        } else {
            m_str.append(1, '1');
        }
    }
    pos = m_str.find(jf1);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf2);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(jf3);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 1) == false)
            four++;
    }
    pos = m_str.find(cf1);
    if (pos != m_str.npos) {
        if (this->ff_special_case(m_str, pos, 2) == false)
            four++;
    } else {
        pos = m_str.find(cf2);
        if (pos != m_str.npos) {
            if (this->ff_special_case(m_str, pos, 3) == false)
                four++;
        }
    }
    if (four > 1)
        return true;
    return false;
}

bool Board::five_connect() {
    int h = this->preAction / 15;
    int w = this->preAction % 15;
    int last_player = this->currentPlayer ^ 1;
    int last_move = this->preAction;
    int i, j;
    int ret;
    if (this->preAction == -1)
        return false;
    int bias = min(w, 4);
    for (i = last_move - bias; i < last_move + 1; i++) {
        if (15 - 1 - i % (15) < 4) {
            break;
        }
        ret = 0;
        for (j = i; j < i + 5; j++) {

            if (this->state[j] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) {
            return true;
        }
    }
    bias = min(h, 4);
    for (i = last_move - bias * 15; i < last_move + 15;
         i += 15) {
        if (15 - 1 - i / 15 < 4) {
            break;
        }
        ret = 0;
        for (j = i; j < i + 5 * 15; j += 15) {

            if (this->state[j] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) {
            return true;
        }
    }
    bias = min(min(h, 4), min(w, 4));
    for (i = last_move - bias * 15 - bias; i < last_move + 15 + 1;
         i += 15 + 1) {
        if ((15 - 1 - i / 15 < 4) ||
            (15 - 1 - i % 15 < 4)) {
            break;
        }
        ret = 0;
        for (j = i; j < i + 5 * 15 + 5; j += 15 + 1) {

            if (this->state[j] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) {
            return true;
        }
    }
    bias = std::min(std::min(15 - 1 - h, 4), std::min(w, 4));
    for (i = last_move + bias * 15 - bias; i > last_move - 15 + 1;
         i = i - 15 + 1) {
        if ((15 - 1 - i % 15 < 4) || (i / 15 < 4)) {
            break;
        }
        ret = 0;
        for (j = i; j > i - 5 * 15 + 5; j = j - 15 + 1) {
            if (this->state[j] != last_player) {
                ret = 1;
                break;
            }
        }
        if (ret == 0) {
            return true;
        }
    }
    return false;
}