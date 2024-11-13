#ifndef MYMENU_H
#define MYMENU_H
#include<vector>
// 实现按钮函数
class myButton {
    public:
        int dx1, dx2, dy1, dy2;
        // 是否被点击到
        bool isBeTouch(int dx, int dy);
        bool leftPut();
        // 一个普遍调用的绘图函数
        void print();
};

std::vector<myButton> getAllButton();
#endif