#include <json.h>
#include <judge.h>
#include <iostream>

using namespace myBoard;

const int BLACK = 1;  // 棋盘为黑
const int WHITE = -1; // 棋盘为白
const int EMPTY = 0;  // 棋盘为空

const int CX[] = {-1, 1, 0, 0, 1, -1, -1, 1};
const int CY[] = {0, 0, -1, 1, 1, -1, 1, -1};
int currentPlayer = BLACK; // 当前玩家
int board[15][15] = {0};   // 当前棋盘

bool inBoard(int x, int y) { return x >= 0 && y >= 0 && x < 15 && y < 15; }

int connectLen(int color, int locx, int locy, int dx, int dy) {
    if (!inBoard(locx, locy) || board[locx][locy] != color)
        return 0;
    int x = locx, y = locy;
    int conNum = 1;
    while (1) {
        x += dx;
        y += dy;
        if (!inBoard(x, y))
            break;
        if (board[x][y] == color)
            conNum++;
        else
            break;
    }
    x = locx, y = locy;
    while (1) {
        x -= dx;
        y -= dy;
        if (!inBoard(x, y))
            break;
        if (board[x][y] == color)
            conNum++;
        else
            break;
    }

    return conNum;
}
bool isFive(int locx, int locy) {
    int color = board[locx][locy];
    for (int i = 0; i < 4; i++) {
        int dx = CX[2 * i], dy = CY[2 * i];
        int conNum = connectLen(color, locx, locy, dx, dy);
        if (conNum >= 5)
            return true;
    }
    return false;
}

bool isSymmetry() {
    for (int x = 0; x < 15; x++)
        for (int y = 0; y < 15; y++) {
            if (board[x][y] != -board[15 - x - 1][15 - y - 1])
                return false;
        }
    return true;
}
int movenum() {
    int n = 0;
    for (int x = 0; x < 15; x++)
        for (int y = 0; y < 15; y++) {
            if (board[x][y] != 0)
                n++;
        }
    return n;
}

bool ProcStep(Board *b, int x, int y, int player, bool checkOnly = false) {
    if (!inBoard(x, y) || board[x][y])
        return false;
    if (!checkOnly) {
        board[x][y] = player;
        b->doAction(x * 15 + y);
    }
    //    if(player == BLACK){
    //        if (b->long_connect()) return false;
    //        if (b->five_connect()) return true;
    //        if (b->three_three()) return false;
    //        if (b->four_four()) return false;
    //    }
    return true;
}

int main() {
    std::string str;
    std::getline(std::cin, str);
    Json::Reader reader;
    Json::Value input, output;
    reader.parse(str, input);
    input = input["log"];
    Board *b = new Board();
    currentPlayer = 1; // 先手为黑
    if (input.size() == 0) {
        output["display"] = "";
        output["command"] = "request";
        output["content"]["0"]["x"] = -1;
        output["content"]["0"]["y"] = -1;
    } else {
        for (int i = 1; i < input.size(); i += 2) {
            bool isLast = i == input.size() - 1;
            Json::Value content;
            Json::Value display;
            if (currentPlayer == 1) // 0号玩家 / 黑方
            {
                Json::Value answer = input[i]["0"]["response"].isNull()
                                         ? input[i]["0"]["content"]
                                         : input[i]["0"]["response"];
                if (((answer.isString() &&
                      reader.parse(answer.asString(), content)) ||
                     (answer.isObject() && (content = answer, true))) &&
                    content["x"].isInt() &&
                    content["y"].isInt()) // 保证输入格式正确
                {
                    int currX = content["x"].asInt();
                    int currY = content["y"].asInt();
                    if ((!ProcStep(b, currX, currY, currentPlayer) &&
                         isLast)) // 不合法棋步！
                    {
                        std::stringstream ss;
                        ss << "INVALID_MOVE  (" << currX << "," << currY << ")";
                        if (inBoard(currX, currY) &&
                            board[currX][currY] != EMPTY)
                            ss << " is not empty";
                        else
                            ss << " first step is not (B, 3)";
                        std::string s;
                        std::getline(ss, s);
                        output["display"]["err"] = s;
                        output["display"]["winner"] = 1;
                        output["command"] = "finish"; // 判输
                        output["content"]["0"] = 0;
                        output["content"]["1"] = 1;
                    } else if (isLast) // 正常棋步
                    {
                        output["display"]["x"] = currX;
                        output["display"]["y"] = currY;
                        output["display"]["color"] = 0;
                        if (b->isGameOver() != b->notOver) {
                            if (b->isGameOver() == b->BLACK) {
                                output["display"]["winner"] = 0;
                                output["command"] = "finish";
                                output["content"]["0"] = 1;
                                output["content"]["1"] = 0;
                            } else if (b->isGameOver() == b->WHITE) {
                                output["display"]["winner"] = 1;
                                output["command"] = "finish";
                                output["content"]["0"] = 0;
                                output["content"]["1"] = 1;
                            } else {
                                output["display"]["winner"] = "none";
                                output["command"] = "finish";
                                output["content"]["0"] = 0.5;
                                output["content"]["1"] = 0.5;
                            }
                        }
                        //                        if (isFive(currX,currY))
                        //                        //黑连五，白胜
                        //                        {
                        //                            output["display"]["winner"]
                        //                            = 1; output["command"] =
                        //                            "finish";
                        //                            output["content"]["0"] =
                        //                            0; output["content"]["1"]
                        //                            = 1;
                        //                        }
                        //                        else if
                        //                        (movenum()>=15*15)
                        //                        //和棋
                        //                        {
                        //                            output["display"]["winner"]
                        //                            = "none";
                        //                            output["command"] =
                        //                            "finish";
                        //                            output["content"]["0"] =
                        //                            0.5;
                        //                            output["content"]["1"] =
                        //                            0.5;
                        //                        }
                        else {
                            output["command"] = "request";
                            output["content"]["1"]["x"] = currX;
                            output["content"]["1"]["y"] = currY;
                        }
                    }
                } else if (isLast) {
                    output["display"]["err"] =
                        "INVALID_INPUT_VERDICT_" +
                        input[i]["0"]["verdict"].asString();
                    output["display"]["winner"] = 1;
                    output["command"] = "finish"; // 判输
                    output["content"]["0"] = 0;
                    output["content"]["1"] = 1;
                }
            } else {
                Json::Value answer = input[i]["1"]["response"].isNull()
                                         ? input[i]["1"]["content"]
                                         : input[i]["1"]["response"];
                if (((answer.isString() &&
                      reader.parse(answer.asString(), content)) ||
                     (answer.isObject() && (content = answer, true))) &&
                    content["x"].isInt() &&
                    content["y"].isInt()) // 保证输入格式正确
                {
                    int currX = content["x"].asInt();
                    int currY = content["y"].asInt();
                    if (!ProcStep(b, currX, currY, currentPlayer) &&
                        isLast) // 不合法棋步！
                    {
                        std::stringstream ss;
                        ss << "INVALID_MOVE  (" << currX << "," << currY << ")";
                        if (inBoard(currX, currY) &&
                            board[currX][currY] != EMPTY)
                            ss << " is not empty";
                        std::string s;
                        std::getline(ss, s);
                        output["display"]["err"] = s;
                        output["display"]["winner"] = 0;
                        output["command"] = "finish"; // 判输
                        output["content"]["0"] = 1;
                        output["content"]["1"] = 0;
                    } else if (isLast) // 正常棋步
                    {
                        output["display"]["x"] = currX;
                        output["display"]["y"] = currY;
                        output["display"]["color"] = 1;
                        if (b->isGameOver() != b->notOver) {
                            if (b->isGameOver() == b->BLACK) {
                                output["display"]["winner"] = 0;
                                output["command"] = "finish";
                                output["content"]["0"] = 1;
                                output["content"]["1"] = 0;
                            } else if (b->isGameOver() == b->WHITE) {
                                output["display"]["winner"] = 1;
                                output["command"] = "finish";
                                output["content"]["0"] = 0;
                                output["content"]["1"] = 1;
                            } else {
                                output["display"]["winner"] = "none";
                                output["command"] = "finish";
                                output["content"]["0"] = 0.5;
                                output["content"]["1"] = 0.5;
                            }
                        }
                        //                        if (isFive(currX,currY))
                        //                        //白连五，黑胜
                        //                        {
                        //                            output["display"]["winner"]
                        //                            = 0; output["command"] =
                        //                            "finish";
                        //                            output["content"]["0"] =
                        //                            1; output["content"]["1"]
                        //                            = 0;
                        //                        }
                        //                        else if (movenum() >= 30 &&
                        //                        isSymmetry())//白棋模仿棋，黑胜
                        //                        {
                        //                            output["display"]["whitesymmetry"]
                        //                            = true;
                        //                            output["display"]["winner"]
                        //                            = 0; output["command"] =
                        //                            "finish";
                        //                            output["content"]["0"] =
                        //                            1; output["content"]["1"]
                        //                            = 0;
                        //                        }
                        else {
                            output["command"] = "request";
                            output["content"]["0"]["x"] = currX;
                            output["content"]["0"]["y"] = currY;
                        }
                    }
                } else if (isLast) {
                    output["display"]["err"] =
                        "INVALID_INPUT_VERDICT_" +
                        input[i]["1"]["verdict"].asString();
                    output["display"]["winner"] = 0;
                    output["command"] = "finish"; // 判输
                    output["content"]["0"] = 1;
                    output["content"]["1"] = 0;
                }
            }
            currentPlayer *= -1;
        }
    }
    Json::FastWriter writer;
    std::cout << writer.write(output) << std::endl;
    return 0;
}