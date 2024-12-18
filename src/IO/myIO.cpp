#include <codecvt>
#include <ctime>
#include <fstream>
#include <game.h>
#include <iomanip>
#include <iostream>
#include <myIO.h>
#include <shlobj.h>
#include <string>
#include <windows.h>
using namespace my_io;
namespace my_io {
// 需要链接 Shell32.lib 库
#pragma comment(lib, "Shell32.lib")
std::string fileDirection;
std::string nFileDirecton;

std::string getNFileDirection() {
    return nFileDirecton;
}

std::string WStringToString(const std::wstring &wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

// 转换为std::string，因为GetOpenFileNameW使用宽字符
std::string SelectFile() {
    OPENFILENAMEA ofn;     // common dialog box structure for ANSI
    char szFile[MAX_PATH]; // buffer for file name
    std::string fileName;  // result string
    // Initialize OPENFILENAMEA
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;                        // starting directory
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Options
    // Display the Open dialog box.
    if (GetOpenFileNameA(&ofn) == TRUE) {
        // Build the full path to the file
        fileName = ofn.lpstrFile;
    }
    return fileName;
}

std::string SelectFolder() {
    BROWSEINFO bi;
    ITEMIDLIST *pidl = nullptr;
    char szPath[MAX_PATH] = {0};
    LPMALLOC pMalloc = nullptr;

    // 初始化 BROWSEINFO 结构
    ZeroMemory(&bi, sizeof(bi));
    bi.hwndOwner = NULL; // 父窗口句柄（通常为 NULL）
    bi.pidlRoot = NULL; // 根文件夹（通常为 NULL 以显示整个文件系统）
    bi.pszDisplayName = szPath; // 用于接收选定文件夹路径的缓冲区
    bi.lpszTitle = "Select a Folder"; // 对话框标题
    bi.ulFlags =
        BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_USENEWUI; // 对话框选项
    bi.lpfn = NULL; // 回调函数（未使用）
    bi.lParam = 0;  // 传递给回调函数的参数（未使用）
    // 显示文件夹选择对话框
    pidl = SHBrowseForFolder(&bi);
    // 如果选择了文件夹
    if (pidl != nullptr) {
        // 从 ITEMIDLIST 获取文件夹路径
        if (SHGetPathFromIDList(pidl, szPath)) {
            // 释放 ITEMIDLIST 内存
            if (SUCCEEDED(SHGetMalloc(&pMalloc))) {
                pMalloc->Free(pidl);
                pMalloc->Release();
            }
            // 返回文件夹路径
            return std::string(szPath);
        }
    }
    // 如果没有选择文件夹或发生错误，则返回空字符串
    return std::string();
}

std::string getCurrentDate() {
    // 获取当前时间点
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    // 创建一个字符串流来格式化日期
    std::ostringstream oss;
    // 将年、月、日添加到字符串流中
    // 注意：年份通常需要加1900，月份需要加1（因为tm_mon的范围是0-11）
    oss << std::put_time(&tm, "%Y%m%d%H%M%S");
    // 返回格式化后的日期字符串
    return oss.str();
}

void initiation() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string fullPath(buffer);
    // 找到最后一个反斜杠的位置
    size_t pos = fullPath.find_last_of("\\");
    // 提取路径部分
    fileDirection = fullPath.substr(0, pos) + "\\";
    nFileDirecton = fileDirection;
}

void modifyDir(std::string s) { fileDirection = s; }

// 校验码
const std::string code = "aZ9dFp3eG8hJk2LmNq1O0WbVcRx4Tu7YSiP";

std::wstring string_to_wstring(const std::string& str, int codepage = CP_UTF8) {
    int len = MultiByteToWideChar(codepage, 0, str.c_str(), -1, nullptr, 0);
    if (len == 0) {
        // 处理错误
        return std::wstring();
    }
    std::vector<wchar_t> buf(len);
    MultiByteToWideChar(codepage, 0, str.c_str(), -1, buf.data(), len);
    return std::wstring(buf.begin(), buf.end() - 1); 
    // 减1是因为MultiByteToWideChar会在末尾添加一个null终止符
}

// 存储方式很简单，每行一个操作即可
std::wstring writeToF(myBoard::Board *b, int kind) {
    std::string fileN = fileDirection + getCurrentDate();
    printf("FileDir:%s\nIn file:%s\n", fileDirection.c_str(), fileN.c_str());
    std::fstream file;
    file.open(fileN, std::ios::out);
    if (!file)
        return L"";
    // 写入校验码内置
    file << code << std::endl;
    // 写入游戏类型
    file << kind << std::endl;
    for (int i = 0; i < b->numPieces; i++)
        file << b->undoState[i] << std::endl;
    file.close();
    return string_to_wstring(fileN);
}

// 提取后返回的是操作序列，约定第一个是游戏类型,-7表示文件不合法
std::vector<int> readFromF(std::string fileN) {
    std::fstream file;
    std::vector<int> ans;
    file.open(fileN, std::ios::in);
    std::string nowCode;
    file >> nowCode;
    if (nowCode != code) {
        ans.push_back(-7); // -7代表不合法
        return ans;
    }
    int action;
    file >> action, ans.push_back(action);
    while (file >> action)
        ans.push_back(action);
    file.close();
    return ans;
}
} // namespace my_io