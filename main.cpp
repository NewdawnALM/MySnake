#include "WinAPI.h"
#include "Info.h"
#include "Map.h"
//#include "Node.h"
#include "Snake.h"

template<typename T>
inline T myabs(const T &x) {
    return x < 0 ? -x: x;
}

int main()
{
    hidden(hOut);
    initColor();

    SetConsoleTitle("  贪吃蛇 1.1");
    system("mode con cols=100 lines=40");

    GameOver = 0;
    score = 0;
    gotoxy(75, 4);
    puts("分数：0");
    gotoxy(75, 6);
    puts("关数：1");
    gotoxy(75, 8);
    puts("方向键 ↑↓ ← →");
    gotoxy(78, 10);
    puts("控制蛇的前进方向");
    gotoxy(75, 12);
    puts("按 p 键暂停或继续，");
    gotoxy(78, 14);
    puts("按 Esc 键退出游戏");

    Snake s(3);
    s.paintBorder();
    s.addWall(2);
    s.reloadCandy();
    s.print();

    while(true) {
        while(kbhit()) {
            s.move(getch());
            if(GameOver == true)   break;
        }
        if(GameOver == true) {
            gotoxy(20, 10);
            setColor(WHITE);
            puts("GameOver");
            gotoxy(0, 33);
            break;
        }
        s.move();
    }
    getch();
    getch();
    setColor(WHITE);
	return 0;
}
