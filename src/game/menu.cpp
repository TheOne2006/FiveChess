#include <game.h>
#include <graphics.h>
#include <menu.h>

bool myButton::isBeTouch(int dx, int dy) {
    return (dx >= dx1 && dx <= dx2 && dy >= dy1 && dy <= dy2);
}

class NextStepButton : public myButton {
  public:
    NextStepButton() {
        dx1 = 40, dy1 = 535, dx2 = 80, dy2 = 555;
        text = "Next";
    }
    bool leftPut() { return nextDo(); }

    void print() {
        TCHAR s[] = _T("Next");
        outtextxy(dx1, dy1, s);
    }
};

class LastStepButton : public myButton {
  public:
    LastStepButton() {
        dx1 = 0, dy1 = 535, dx2 = 40, dy2 = 555;
        text = "Last";
    }

    bool leftPut() { return undo(); }

    void print() {
        TCHAR s[] = _T("Last");
        outtextxy(dx1, dy1, s);
    }
};

class WriteButton : public myButton {
  public:
    WriteButton() {
        dx1 = 80, dy1 = 535, dx2 = 120, dy2 = 555;
        text = "Write";
    }
    bool leftPut() {}

    void print() {
        TCHAR s[] = _T("Write");
        outtextxy(dx1, dy1, s);
    }
};

class ReadButton : public myButton {
  public:
    ReadButton() {
        dx1 = 120, dy1 = 535, dx2 = 160, dy2 = 555;
        text = "Read";
    }

    bool leftPut() {}

    void print() {
        TCHAR s[] = _T("Read");
        outtextxy(dx1, dy1, s);
    }
};

std::vector<myButton *> getAllButton() {
    std::vector<myButton *> ans;
    ans.push_back(new NextStepButton());
    ans.push_back(new LastStepButton());
    ans.push_back(new WriteButton());
    ans.push_back(new ReadButton());
    return ans;
}

bool myButton::leftPut() { return false; }
// 一个普遍调用的绘图函数
void myButton::print() { return; }