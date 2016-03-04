#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <algorithm>

namespace _color
{
    const WORD black = 0;
    const WORD blue = 1;
    const WORD green = 2;
    const WORD cyan = 3;
    const WORD red = 4;
    const WORD purple = 5;
    const WORD yellow = 6;
    const WORD white = 7;
    const WORD BLACK = 8;
    const WORD BLUE = 9;
    const WORD GREEN = 10;
    const WORD CYAN = 11;
    const WORD RED = 12;
    const WORD PURPLE = 13;
    const WORD YELLOW = 14;
    const WORD WHITE = 15;
}


// 用个命名空间封装起所有信息

namespace info {

    const char *snake = "●";
    const char *space = "  ";
    const char *wall = "■";
    const char *board_bottom = "▃";
    const char *board_side = "▌";

    const short border_top = 0;
    const short border_down = 30;
    const short border_left = 0;
    const short border_right = 70;

    const char key_up = 72;
    const char key_down = 80;
    const char key_left = 75;
    const char key_right = 77;
    const char key_p = 112;
    const char key_Esc = 27;
    const char key_Enter = 13;

    const short stop = 0;
    const short up = 1;
    const short left = 2;
    const short right = 3;
    const short down = 4;

    const short step = strlen("●");
    const short direct[6][2]= {{}, {0,-1}, {-step,0}, {step,0}, {0,1} };

    const static short default_sleep_time = 300;

    short reverse_dir(short dir) {
        if(dir == up)   return down;
        if(dir == down)    return up;
        if(dir == left)    return right;
        if(dir == right)    return left; 
    }
}
