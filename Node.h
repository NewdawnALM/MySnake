
using namespace info;

// 节点类：
class node {
    const char *snake_char;       // 节点的字符
    short step;             // 节点的长度（横向）
    COORD cd;              // 节点所在的坐标（用来在屏幕上对应的位置绘画出来）
    short dir = stop;           // 节点前进的方向，也表示前一个节点在哪个方向
    bool virt = 0;              // 是否为虚拟节点（不会画出来）
public:
    node(const char *ch = "●"): snake_char(ch) {
        this->step = strlen(ch);
    }
    node operator = (const node &n2) {
        snake_char = n2.snake_char;
        step = n2.step;
        cd = n2.cd;
        dir = n2.dir;
        virt = n2.virt;
        return *this;
    }
    ~node() { }

    COORD getxy() const {  return cd;   }          //获取节点所在坐标
    short getdir() const {  return dir;   }         //获取节点的前进方向

    short length() const {   return step;   }

    void setxy(SHORT x, SHORT y) {               // 设置节点坐标
        cd.X = x;
        cd.Y = y;
    }
    void setxy(COORD _cd) {
        setxy(_cd.X,_cd.Y);
    }

    void setdir(short _dir) {                   //设置节点前进方向
        this->dir = _dir;
    }

    void setvirt(bool v) {               //设置是否为虚拟结点
        this->virt = v;
    }

    void reset_virt() {                  // 设置为虚拟节点或把虚拟节点设置为实际的节点
        virt = !virt;
    }

    COORD nextxy(short dir) const {              // 获得节点前一个节点的坐标，由方向 dir 来确定
        return COORD { cd.X + direct[dir][0], cd.Y + direct[dir][1] };
    }

    void print(DWORD time = info::default_sleep_time) const {         //在屏幕上绘画出该节点
        COORD current_cursor;
        Getxy(current_cursor);              // 获取当前光标位置
        gotoxy(this->cd);
        if(!virt)    printf("%s", snake_char);       // 不是虚拟结点的话就直接画出来
        else    printf("%s", space);         // 否则隐藏起来
        Sleep(time);
        gotoxy(current_cursor);            // 光标回到原来的位置，为了使这个函数对外界不产生其它的影响
    }
};
