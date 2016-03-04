#include <vector>
using std::vector;

bool GameOver;
int score;
short level = 1;
short wall_num = 2;

using namespace _color;

class Snake: public _map {
    short len;                  // 蛇的长度
    vector<node> v;             // 用 vector 来保存蛇的节点，为了增长操作的方便
    vector<short> index;        // 保存各节点在 vector<node> 中的索引（为了准确存取 tail）
    short tail, head;           // 分别表示蛇的首尾
    DWORD time_interval = 100;                 // 移动的时间间隔
public:
    Snake(short len = 6): len(len)  {    init();   }

    // 初始化蛇的各个结点的信息
    void init() {
        v.resize(this->len);          // 设置蛇的长度
        index.clear();
        index.push_back(0);
        v[0].setxy(0, 0);                 // v[0] 为尾结点
        v[0].setdir(right);
        setMap(COORD{0, 0}, barrier, v[0].length());

        tail = 0;                   // 设置 tail 结点在 vector 中的下标为 0；head 结点则为 len - 1
        head = len - 1;
        for(auto i = tail + 1; i <= head; ++i) {        // 从尾到头设好各个结点的坐标和方向
            index.push_back(i);                    // 把各个节点的序号从 tail 到 head 依次加入到 index 中
            COORD behind = v[i - 1].getxy();
            short dir = v[i - 1].getdir();
            short x = behind.X + direct[dir][0], y = behind.Y + direct[dir][1];
            v[i].setxy(x, y);
            v[i].setdir(dir);
            setMap(COORD{x, y}, barrier, v[i].length());
        }
    }

    // 画出整条蛇出来（游戏一开始时）
    void print() const {
        setColor(YELLOW);
        for(int i = tail; i <= head; ++i)
            v[i].print(time_interval);
    }

    // 增加 cnt 堵墙的数量
    void addWall(short cnt = 2) {
        if(cnt == 0)   return ;

        srand(time(0));
        // 随机生成墙壁的第一个结点
        short x = rand() % (border_right - border_left - 1) + border_left + 1;
        short y = rand() % (border_down - border_top - 1) + border_top + 1;
        // 为了找到符合条件的墙壁起始点 n(x,y)
        // 至关重要的 ( x & 1 ), 为了生成的墙壁与贪吃蛇不错位，即 node 点的起始位置一定要为偶数！
        while( x & 1 || isCrash( COORD{x, y} ) ) {
            x = rand() % (border_right - border_left - 1) + border_left + 1;
            y = rand() % (border_down - border_top - 1) + border_top + 1;
        }
        // 画出这个结点并在地图对应位置设置好障碍
        node n(wall);
        n.setxy(x, y);
        n.print(0);
        setMap(COORD{x, y}, barrier, n.length());
        wall_node.push_back(n);

        // 随机生成墙壁结点扩展的数量
        short num = rand() % 6 + 5;
        while(num--) {
            short dir = (rand() & 3 ) + 1;          // 随机生成一个扩展的方向
            // 判断向这个方向扩展是否可行，否则更改方向；为了防止 4 个方向都不可行，所以用个变量 i 来限定最多循环 4 次
            for(auto i = 1; isCrash(n.nextxy(dir)) && i <= 4 ; ++i) {
                ++dir;
                dir &= 3;        // 相当于模 4 操作而已
            }
            // 新生成的结点所在位置不是障碍物的话就画出来同时设置好障碍标记
            if( !isCrash(n.nextxy(dir)) ) {
                n.setxy(n.nextxy(dir));         // 这儿的顺序给我搞好了！先 setxy 到正确的位置
                n.print(0);                       // 然后再画出墙
                setMap(n.getxy(), barrier, n.length());         // 最后再对地图设置障碍，曾经因为顺序错调试了n久(T.T)
                wall_node.push_back(n);
            }
        }
        addWall(cnt - 1);
    }

    // 随机生成糖果的位置
    void reloadCandy() {
        srand(time(0));
        short x = rand() % (border_right - border_left - 1) + border_left + 1;
        short y = rand() % (border_down - border_top - 1) + border_top + 1;

        // 和上面一样的原理
        while( x & 1 || isCrash( COORD{x,y} ) ) {
            x = rand() % (border_right - border_left - 1) + border_left + 1;
            y = rand() % (border_down - border_top - 1) + border_top + 1;
        }
        node n;
        n.setxy(x, y);
        setColor(RED);
        n.print(0);
        setColor(WHITE);
        setMap(COORD{x, y}, candy, n.length());
    }

    void setTime_interval(DWORD time) {
        this->time_interval = time;
    }

    void updateLevel(int score) {
        if(score % 50 == 0) {
            clearAllWall();
            addWall(++wall_num);
            ++level;
            setColor(CYAN);
            for(auto i = 1; i <= 3; ++i) {
                gotoxy(75, 6);
                printf("         ");
                Sleep(500);
                gotoxy(75, 6);
                printf("关数：%d", level);
                Sleep(500);
            }
            setColor(WHITE);
            gotoxy(75, 6);
            printf("关数：%d", level);
        }
    }

    // 移动函数，每次重绘头和尾，并进行碰撞检测，可以说是游戏核心部分啊~
    void move() {
        node n;
        n.setxy(v[head].nextxy(v[head].getdir()));       // n 代表 head 方向前的节点
        n.setdir(v[head].getdir());

        if( isCrash(n.getxy()) ) {          // 如果这个节点(即因前进产生的新节点)对应的位置会发生碰撞，则游戏结束
            GameOver = true;
            return ;
        }

        setColor(YELLOW);
        n.print(time_interval);             // 绘出这个节点，也就是新的 head 节点

        if( isCandy(n.getxy()) ) {          // 如果为糖果的话，作相应的处理：分数增加，蛇身变长等
            score += 10;
            gotoxy(75, 4);
            setColor(WHITE);
            printf("分数：%d", score);
            updateLevel(score);

            index.push_back(len);       // 把新增节点的序号放进 index 的尾部
            v.push_back(n);             // 同理加进 v 中
            len = v.size();
            head = len - 1;         // 更新 head 的值，也就是指向新增的序号；而此时因为吃到糖果的原因 tail 不用擦除，所以关于 tail 的一切信息都不用更新

            // 这一句很有必要！在 reloadCandy 前必须在 v[head] 处设好障碍标记
            setMap(v[head].getxy(), barrier, v[head].length());         // setMap(n.getxy(), barrier, n.length()) 也行
            reloadCandy();                  // 重新生成糖果的位置
        }

        // 吃不到糖果的话也就是普通的移动了，此时需要擦除 tail
        else {
            v[tail].setvirt(true);          // 把 tail 设置为虚拟结点
            v[tail].print(time_interval);           // 然后重新绘制，即擦除
            setMap(v[tail].getxy(), space, v[tail].length());       // 对应位置设置为可行结点

            index.erase(index.begin());         // index.begin() 即 tail 在 v 中的索引，把它从 index 的首部取出，
            index.push_back(tail);              // 然后放到 index 的末尾，
            v[tail] = n;                    // 然后把前进生成的新节点赋值给 tail，此时 v[tail] 存放的就是新的头结点
            head = tail;                    // 把 head 指向 tail
            tail = index[0];                // 最后 tail 回到蛇移动后的尾巴，也就是此时的 index[0]

            setMap(v[head].getxy(), barrier, v[head].length());
            // setMap(n.getxy(), barrier, n.length()) 也行
        }
    }

    // 改变蛇的前进方向，也就是 head 结点指向的方向
    void setdir(short _dir) {
        v[head].setdir(_dir);
    }

    bool check_key(char ch) {
        if(ch == key_Esc) {
            setColor(WHITE);
            gotoxy(6, 32);
            printf("是否退出游戏？y / n ： ");
            char ch = getch();
            if(ch == 'y' || ch == 'Y' || ch == key_Enter) {
                gotoxy(6, 32);
                printf("游戏结束！             \n\n");
                Sleep(800);
                exit(0);
            }
            else {
                gotoxy(6, 32);
                printf("                           ");
                return false;
            }
        }
        if(ch == key_up || ch == key_left || ch == key_right || ch == key_down || ch == key_p)
            return true;
        return false;
    }

    void move(char ch) {
        if(!check_key(ch))   return ;       // 至关重要的一句代码（方向键返回 -32 和本身的 72,75,80…… ，会返回两次的！）
        if(ch == key_p) {
            ch = getch();
            while(ch != key_p)    ch = getch();
            return ;
        }
        short dir = v[head].getdir();
        if(ch == key_up && dir != down)    v[head].setdir(up);
        else if(ch == key_down && dir != up)   v[head].setdir(down);
        else if(ch == key_left && dir != right)   v[head].setdir(left);
        else if(ch == key_right && dir != left)   v[head].setdir(right);
        move();
    }
};
