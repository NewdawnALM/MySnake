#include <windows.h>

//��׼������������Ϊȫ�ֱ���������һЩ
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

//���ع��
void hidden(HANDLE &hOut)
{
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut,&cci);
    cci.bVisible = 0;
    SetConsoleCursorInfo(hOut,&cci);
}

//���ù���λ�����ں��ʵ�λ�������Ϣ
inline void gotoxy(COORD cd, HANDLE h = hOut){
    SetConsoleCursorPosition(h,cd);
}

inline void gotoxy(SHORT x, SHORT y, HANDLE h = hOut){
    gotoxy(COORD{x,y},h);
}

//��ÿ���̨���λ��
void Getxy(SHORT &x, SHORT &y, HANDLE h = hOut) {
    CONSOLE_SCREEN_BUFFER_INFO p;
    GetConsoleScreenBufferInfo(h,&p);
    x = p.dwCursorPosition.X;
    y = p.dwCursorPosition.Y;
}

void Getxy(COORD &cd, HANDLE h = hOut) {
    Getxy(cd.X,cd.Y,h);
}

WORD color[16];        //��ɫ����
void initColor() {
    // ���� windows.h�����ɫ���ý�����ɫ��
    // �� 0 -- 7 �ֱ�Ϊ���ڣ������̣������̣��죬�����������̣���
    // �� 8 -- 15 �ֱ�Ϊ���ڣ������̣������̣��죬�����������̣��ף������ϸ�������ʾ��ǰ��ɫ
    for(WORD i = 0; i < 8; ++i) {
        color[i] = i;
        color[i + 8] = i | FOREGROUND_INTENSITY;
    }
}

void setColor(WORD c, HANDLE h = hOut) {
    SetConsoleTextAttribute(h, color[c]);
}
