#include <conio.h>
#include <easyx.h>
#include <game.h>
#include <graphics.h>
#include <judge.h>
#include <myIO.h>
#include <menu.h>
#include <setting.h>
using namespace my_setting;
namespace my_setting {
IMAGE settingFace;
std::vector<myButton *> buttons;

void initiation() {
    closegraph();
    initgraph(360, 640);
}

void print() {
    cleardevice();
    putimage(0, 0, &settingFace);
    for (int i = 0; i < buttons.size(); i++)
        buttons[i]->print(false);
}

class FileButton : public myButton {
  public:
    FileButton() {
        dx1 = 142, dy1 = 414, dx2 = 242, dy2 = 449;
        text = "File";
    }
    bool leftPut() {
        printf("Choose the file\n");
        my_io::modifyDir(my_io::SelectFolder() + "\\");
        return false;
    }
};

class SimpleButton : public myButton {
  private:
    const int depth = 2;

  public:
    SimpleButton() {
        dx1 = 260, dy1 = 300, dx2 = 293, dy2 = 315;
        text = "Simple";
    }
    bool leftPut() {
        printf("Choose the simple level\n");
        modifyDepth(depth);
        return false;
    }
    void print(bool f) {
        if (getDepth() == depth) {
            setfillcolor(RED);
            solidcircle((dx1 + dx2) / 2, dy2 + 3, 4);
        }
    }
};

class ComplexButton : public myButton {
  private:
    const int depth = 3;

  public:
    ComplexButton() {
        dx1 = 200, dy1 = 300, dx2 = 231, dy2 = 315;
        text = "Complex";
    }
    bool leftPut() {
        printf("Choose the complex level\n");
        modifyDepth(depth);
        return false;
    }
    void print(bool f) {
        if (getDepth() == depth) {
            setfillcolor(RED);
            solidcircle((dx1 + dx2) / 2, dy2 + 3, 4);
        }
    }
};

class MasterButton : public myButton {
  private:
    const int depth = 5;

  public:
    MasterButton() {
        dx1 = 140, dy1 = 300, dx2 = 169, dy2 = 315;
        text = "Master";
    }
    bool leftPut() {
        printf("Choose the master level\n");
        modifyDepth(depth);
        return false;
    }
    void print(bool f) {
        if (getDepth() == depth) {
            setfillcolor(RED);
            solidcircle((dx1 + dx2) / 2, dy2 + 3, 4);
        }
    }
};

class ReturnButton : public myButton {
  public:
    ReturnButton() {
        dx1 = 310, dy1 = 6, dx2 = 350, dy2 = 33;
        text = "Return";
    }
    bool leftPut() {
        printf("Return to the start\n");
        return true;
    }
};

class WhiteButton : public myButton {
  private:
    const int color = 0;

  public:
    WhiteButton() {
        dx1 = 236, dy1 = 355, dx2 = 268, dy2 = 370;
        text = "White";
    }
    bool leftPut() {
        printf("Choose the white\n");
        modifyAi(color);
        return false;
    }
    void print(bool f) {
        if (getAIColor() == color) {
            setfillcolor(RED);
            solidcircle((dx1 + dx2) / 2, dy2 + 3, 4);
        }
    }
};

class BlackButton : public myButton {
  private:
    const int color = 1;
  public:
    BlackButton() {
        dx1 = 151, dy1 =355, dx2 = 186, dy2 = 370;
        text = "Black";
    }
    bool leftPut() {
        printf("Choose the black\n");
        modifyAi(color);
        return false;
    }
    void print(bool f) {
        if (getAIColor() == color) {
            setfillcolor(RED);
            solidcircle((dx1 + dx2) / 2, dy2 + 3, 4);
        }
    }
};

bool isOnButton(int dx, int dy) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i]->isBeTouch(dx, dy)) {
            bool ans = buttons[i]->leftPut();
            print();
            return ans;
        }
    }
}

bool waitClick() {
    ExMessage m;
    while (true) {
        m = getmessage(EX_MOUSE);
        if (m.message == WM_LBUTTONDOWN) {
            if (isOnButton(m.x, m.y)) {
                return true;
            }
        }
    }
}

bool start() {
    std::string imagePath = my_io::getNFileDirection() + "\\res\\setting.png";
    loadimage(&settingFace, _T(imagePath.c_str()));
    if (buttons.empty()) {
        buttons.push_back(new SimpleButton());
        buttons.push_back(new ComplexButton());
        buttons.push_back(new MasterButton());
        buttons.push_back(new FileButton());
        buttons.push_back(new ReturnButton());
        buttons.push_back(new WhiteButton());
        buttons.push_back(new BlackButton());
    }
    initiation();
    print();
    return waitClick();
}
} // namespace my_setting