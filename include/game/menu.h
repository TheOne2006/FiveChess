#ifndef MYMENU_H
#define MYMENU_H
#include <string>
#include <vector>
// 实现按钮函数
class myButton {
  public:
    // 定位
    int dx1, dx2, dy1, dy2;
    // 文字
    std::string text;
    // 是否被点击到
    bool isBeTouch(int dx, int dy);
    virtual bool leftPut();
    // 一个普遍调用的绘图函数
    virtual void print(bool f);
};

std::vector<myButton *> getAllButton();
#endif