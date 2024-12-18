#include <conio.h>
#include <game.h>
#include <graphics.h>
#include <judge.h>
#include <menu.h>
#include <myIO.h>
#include <setting.h>
#include <start.h>
#include <vector>
using namespace my_start;
namespace my_start {
// 开始游戏按钮(双人游戏)
class TwoPlayButton : public myButton {
  public:
    TwoPlayButton() {
        dx1 = 127, dy1 = 240, dx2 = 243, dy2 = 262;
        text = "Two";
    }
    bool leftPut() {
        printf("Start two game!\n");
        closegraph();
        initiation();
        gameTwoStart();
        return true;
    }
};

// 开始游戏按钮(单人游戏)
class SinglePlayButton : public myButton {
  public:
    SinglePlayButton() {
        dx1 = 127, dy1 = 317, dx2 = 243, dy2 = 344;
        text = "Single";
    }
    bool leftPut() {
        printf("Start single game!\n");
        initiation();
        gameSingleStart();
        return true;
    }
};

// 游戏设置按钮
class SettingButton : public myButton {
  public:
    SettingButton() {
        dx1 = 127, dy1 = 479, dx2 = 243, dy2 = 504;
        text = "Setting";
    }
    bool leftPut() {
        printf("Touch setting button!\n");
        return my_setting::start();
    }
};

// 载入游戏按钮
class LoadButton : public myButton {
  private:
    bool ShowRetryCancelMessageBox(HWND hWndParent = NULL) {
        // 定义Unicode字符串的消息和标题
        const wchar_t *message = L"请选择合适的棋盘文件。要重试吗？";
        const wchar_t *title = L"选择棋盘文件";
        UINT uType = MB_YESNO | MB_ICONQUESTION; // 是/否按钮和问号图标

        // 显示消息框并获取用户响应
        int result = MessageBoxW(hWndParent, message, title, uType);

        // 根据用户响应返回true（重试）或false（取消）
        return (result == IDYES); // 如果用户点击“是”，则返回true；否则返回false
    }

  public:
    LoadButton() {
        dx1 = 127, dy1 = 397, dx2 = 243, dy2 = 426;
        text = "Load";
    }
    bool leftPut() {
        printf("Touch load button!\n");
        bool isBoardFile = true, isGame = false;;
        while (isBoardFile) {
            std::string s = my_io::SelectFile();
            std::vector<int> boardFile = my_io::readFromF(s);
            if (boardFile[0] == -7) {
                isBoardFile = ShowRetryCancelMessageBox();
            } else {
                isBoardFile = false, isGame = true;
                initiation();
                gameLoadStart(boardFile);
            }
        }
        return isGame;
    }
};

// 约定leftput返回值，true表示重启界面。

// 开始界面菜单控制
IMAGE startFace;

void print() {
    closegraph();
    initgraph(360, 640);
    cleardevice();
    putimage(0, 0, &startFace);
}

std::vector<myButton *> buttons;

// -1代表没点中，1表示重新加载界面，0表示不需要重新加载
int isOnButton(int dx, int dy) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i]->isBeTouch(dx, dy))
            return buttons[i]->leftPut();
    }
    return -1;
}

bool waitClick() {
    ExMessage m;
    while (true) {
        m = getmessage(EX_MOUSE);
        if (m.message == WM_LBUTTONDOWN) {
            int isOn = isOnButton(m.x, m.y);
            if (isOn == 1) {
                return true;
            }
        }
    }
}

void start() {
    bool f = true;
    while (true) {
        std::string imagePath = my_io::getNFileDirection() + "\\res\\start.png";
        loadimage(&startFace, _T(imagePath.c_str()));
        if (buttons.empty()) {
            buttons.push_back(new SettingButton());
            buttons.push_back(new TwoPlayButton());
            buttons.push_back(new SinglePlayButton());
            buttons.push_back(new LoadButton());
        }
        print();
        f = waitClick();
    }
}
} // namespace my_start