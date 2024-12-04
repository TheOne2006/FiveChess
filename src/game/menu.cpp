#include <game.h>
#include <graphics.h>
#include <menu.h>
#include <myIO.h>

bool myButton::isBeTouch(int dx, int dy) {
    return (dx >= dx1 && dx <= dx2 && dy >= dy1 && dy <= dy2);
}

class NextStepButton : public myButton {
  public:
    NextStepButton() {
        dx1 = 545, dy1 = 306, dx2 = 600, dy2 = 348;
        text = "Next";
    }
    bool leftPut() {
        printf("Next\n");
        nextDo();
        return false;
    }
};

class LastStepButton : public myButton {
  public:
    LastStepButton() {
        dx1 = 545, dy1 = 214, dx2 = 600, dy2 = 250;
        text = "Last";
    }

    bool leftPut() {
        printf("Undo\n");
        undo();
        return false;
    }
};

class WriteButton : public myButton {
  public:
    WriteButton() {
        dx1 = 545, dy1 = 115, dx2 = 600, dy2 = 154;
        text = "Write";
    }
    bool leftPut() {
        printf("Write to file\n");
        std::wstring message = L"文件已成功保存: " + my_io::writeToF(getCore(), getKind());
        MessageBoxW(NULL, message.c_str(), L"信息", MB_OK);
        return false;
    }
};

class ReturnButton : public myButton {
  public:
    ReturnButton() {
        dx1 = 545, dy1 = 22, dx2 = 600, dy2 = 56;
        text = "Return";
    }
    bool leftPut() {
        printf("Return to the start\n");
        return true;
    }
};

std::vector<myButton *> getAllButton() {
    std::vector<myButton *> ans;
    ans.push_back(new NextStepButton());
    ans.push_back(new LastStepButton());
    ans.push_back(new WriteButton());
    ans.push_back(new ReturnButton());
    return ans;
}

bool myButton::leftPut() { return false; }
// 一个普遍调用的绘图函数
void myButton::print(bool f) { return; }