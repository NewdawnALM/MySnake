#include <vector>
using std::vector;

bool GameOver;
int score;
short level = 1;
short wall_num = 2;

using namespace _color;

class Snake: public _map {
    short len;                  // �ߵĳ���
    vector<node> v;             // �� vector �������ߵĽڵ㣬Ϊ�����������ķ���
    vector<short> index;        // ������ڵ��� vector<node> �е�������Ϊ��׼ȷ��ȡ tail��
    short tail, head;           // �ֱ��ʾ�ߵ���β
    DWORD time_interval = 100;                 // �ƶ���ʱ����
public:
    Snake(short len = 6): len(len)  {    init();   }

    // ��ʼ���ߵĸ���������Ϣ
    void init() {
        v.resize(this->len);          // �����ߵĳ���
        index.clear();
        index.push_back(0);
        v[0].setxy(0, 0);                 // v[0] Ϊβ���
        v[0].setdir(right);
        setMap(COORD{0, 0}, barrier, v[0].length());

        tail = 0;                   // ���� tail ����� vector �е��±�Ϊ 0��head �����Ϊ len - 1
        head = len - 1;
        for(auto i = tail + 1; i <= head; ++i) {        // ��β��ͷ��ø�����������ͷ���
            index.push_back(i);                    // �Ѹ����ڵ����Ŵ� tail �� head ���μ��뵽 index ��
            COORD behind = v[i - 1].getxy();
            short dir = v[i - 1].getdir();
            short x = behind.X + direct[dir][0], y = behind.Y + direct[dir][1];
            v[i].setxy(x, y);
            v[i].setdir(dir);
            setMap(COORD{x, y}, barrier, v[i].length());
        }
    }

    // ���������߳�������Ϸһ��ʼʱ��
    void print() const {
        setColor(YELLOW);
        for(int i = tail; i <= head; ++i)
            v[i].print(time_interval);
    }

    // ���� cnt ��ǽ������
    void addWall(short cnt = 2) {
        if(cnt == 0)   return ;

        srand(time(0));
        // �������ǽ�ڵĵ�һ�����
        short x = rand() % (border_right - border_left - 1) + border_left + 1;
        short y = rand() % (border_down - border_top - 1) + border_top + 1;
        // Ϊ���ҵ�����������ǽ����ʼ�� n(x,y)
        // ������Ҫ�� ( x & 1 ), Ϊ�����ɵ�ǽ����̰���߲���λ���� node �����ʼλ��һ��ҪΪż����
        while( x & 1 || isCrash( COORD{x, y} ) ) {
            x = rand() % (border_right - border_left - 1) + border_left + 1;
            y = rand() % (border_down - border_top - 1) + border_top + 1;
        }
        // ���������㲢�ڵ�ͼ��Ӧλ�����ú��ϰ�
        node n(wall);
        n.setxy(x, y);
        n.print(0);
        setMap(COORD{x, y}, barrier, n.length());
        wall_node.push_back(n);

        // �������ǽ�ڽ����չ������
        short num = rand() % 6 + 5;
        while(num--) {
            short dir = (rand() & 3 ) + 1;          // �������һ����չ�ķ���
            // �ж������������չ�Ƿ���У�������ķ���Ϊ�˷�ֹ 4 �����򶼲����У������ø����� i ���޶����ѭ�� 4 ��
            for(auto i = 1; isCrash(n.nextxy(dir)) && i <= 4 ; ++i) {
                ++dir;
                dir &= 3;        // �൱��ģ 4 ��������
            }
            // �����ɵĽ������λ�ò����ϰ���Ļ��ͻ�����ͬʱ���ú��ϰ����
            if( !isCrash(n.nextxy(dir)) ) {
                n.setxy(n.nextxy(dir));         // �����˳����Ҹ���ˣ��� setxy ����ȷ��λ��
                n.print(0);                       // Ȼ���ٻ���ǽ
                setMap(n.getxy(), barrier, n.length());         // ����ٶԵ�ͼ�����ϰ���������Ϊ˳��������n��(T.T)
                wall_node.push_back(n);
            }
        }
        addWall(cnt - 1);
    }

    // ��������ǹ���λ��
    void reloadCandy() {
        srand(time(0));
        short x = rand() % (border_right - border_left - 1) + border_left + 1;
        short y = rand() % (border_down - border_top - 1) + border_top + 1;

        // ������һ����ԭ��
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
                printf("������%d", level);
                Sleep(500);
            }
            setColor(WHITE);
            gotoxy(75, 6);
            printf("������%d", level);
        }
    }

    // �ƶ�������ÿ���ػ�ͷ��β����������ײ��⣬����˵����Ϸ���Ĳ��ְ�~
    void move() {
        node n;
        n.setxy(v[head].nextxy(v[head].getdir()));       // n ���� head ����ǰ�Ľڵ�
        n.setdir(v[head].getdir());

        if( isCrash(n.getxy()) ) {          // �������ڵ�(����ǰ���������½ڵ�)��Ӧ��λ�ûᷢ����ײ������Ϸ����
            GameOver = true;
            return ;
        }

        setColor(YELLOW);
        n.print(time_interval);             // �������ڵ㣬Ҳ�����µ� head �ڵ�

        if( isCandy(n.getxy()) ) {          // ���Ϊ�ǹ��Ļ�������Ӧ�Ĵ����������ӣ�����䳤��
            score += 10;
            gotoxy(75, 4);
            setColor(WHITE);
            printf("������%d", score);
            updateLevel(score);

            index.push_back(len);       // �������ڵ����ŷŽ� index ��β��
            v.push_back(n);             // ͬ��ӽ� v ��
            len = v.size();
            head = len - 1;         // ���� head ��ֵ��Ҳ����ָ����������ţ�����ʱ��Ϊ�Ե��ǹ���ԭ�� tail ���ò��������Թ��� tail ��һ����Ϣ�����ø���

            // ��һ����б�Ҫ���� reloadCandy ǰ������ v[head] ������ϰ����
            setMap(v[head].getxy(), barrier, v[head].length());         // setMap(n.getxy(), barrier, n.length()) Ҳ��
            reloadCandy();                  // ���������ǹ���λ��
        }

        // �Բ����ǹ��Ļ�Ҳ������ͨ���ƶ��ˣ���ʱ��Ҫ���� tail
        else {
            v[tail].setvirt(true);          // �� tail ����Ϊ������
            v[tail].print(time_interval);           // Ȼ�����»��ƣ�������
            setMap(v[tail].getxy(), space, v[tail].length());       // ��Ӧλ������Ϊ���н��

            index.erase(index.begin());         // index.begin() �� tail �� v �е������������� index ���ײ�ȡ����
            index.push_back(tail);              // Ȼ��ŵ� index ��ĩβ��
            v[tail] = n;                    // Ȼ���ǰ�����ɵ��½ڵ㸳ֵ�� tail����ʱ v[tail] ��ŵľ����µ�ͷ���
            head = tail;                    // �� head ָ�� tail
            tail = index[0];                // ��� tail �ص����ƶ����β�ͣ�Ҳ���Ǵ�ʱ�� index[0]

            setMap(v[head].getxy(), barrier, v[head].length());
            // setMap(n.getxy(), barrier, n.length()) Ҳ��
        }
    }

    // �ı��ߵ�ǰ������Ҳ���� head ���ָ��ķ���
    void setdir(short _dir) {
        v[head].setdir(_dir);
    }

    bool check_key(char ch) {
        if(ch == key_Esc) {
            setColor(WHITE);
            gotoxy(6, 32);
            printf("�Ƿ��˳���Ϸ��y / n �� ");
            char ch = getch();
            if(ch == 'y' || ch == 'Y' || ch == key_Enter) {
                gotoxy(6, 32);
                printf("��Ϸ������             \n\n");
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
        if(!check_key(ch))   return ;       // ������Ҫ��һ����루��������� -32 �ͱ���� 72,75,80���� ���᷵�����εģ���
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
