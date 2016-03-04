#include <windows.h>

//标准输出句柄，设置为全局变量更方便一些
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

//隐藏光标
void hidden(HANDLE &hOut)
{
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut,&cci);
    cci.bVisible = 0;
    SetConsoleCursorInfo(hOut,&cci);
}

//设置光标的位置以在合适的位置输出信息
inline void gotoxy(COORD cd, HANDLE h = hOut){
    SetConsoleCursorPosition(h,cd);
}

inline void gotoxy(SHORT x, SHORT y, HANDLE h = hOut){
    gotoxy(COORD{x,y},h);
}

//获得控制台光标位置
void Getxy(SHORT &x, SHORT &y, HANDLE h = hOut) {
    CONSOLE_SCREEN_BUFFER_INFO p;
    GetConsoleScreenBufferInfo(h,&p);
    x = p.dwCursorPosition.X;
    y = p.dwCursorPosition.Y;
}

void Getxy(COORD &cd, HANDLE h = hOut) {
    Getxy(cd.X,cd.Y,h);
}

WORD color[16];        //颜色数组
void initColor() {
    // 根据 windows.h里的颜色设置进行配色，
    // 从 0 -- 7 分别为：黑，蓝，绿，蓝加绿，红，红加蓝，红加绿，白
    // 从 8 -- 15 分别为：黑，蓝，绿，蓝加绿，红，红加蓝，红加绿，白，再配上高清亮显示的前景色
    for(WORD i = 0; i < 8; ++i) {
        color[i] = i;
        color[i + 8] = i | FOREGROUND_INTENSITY;
    }
}

void setColor(WORD c, HANDLE h = hOut) {
    SetConsoleTextAttribute(h, color[c]);
}
