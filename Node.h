
using namespace info;

// �ڵ��ࣺ
class node {
    const char *snake_char;       // �ڵ���ַ�
    short step;             // �ڵ�ĳ��ȣ�����
    COORD cd;              // �ڵ����ڵ����꣨��������Ļ�϶�Ӧ��λ�û滭������
    short dir = stop;           // �ڵ�ǰ���ķ���Ҳ��ʾǰһ���ڵ����ĸ�����
    bool virt = 0;              // �Ƿ�Ϊ����ڵ㣨���ử������
public:
    node(const char *ch = "��"): snake_char(ch) {
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

    COORD getxy() const {  return cd;   }          //��ȡ�ڵ���������
    short getdir() const {  return dir;   }         //��ȡ�ڵ��ǰ������

    short length() const {   return step;   }

    void setxy(SHORT x, SHORT y) {               // ���ýڵ�����
        cd.X = x;
        cd.Y = y;
    }
    void setxy(COORD _cd) {
        setxy(_cd.X,_cd.Y);
    }

    void setdir(short _dir) {                   //���ýڵ�ǰ������
        this->dir = _dir;
    }

    void setvirt(bool v) {               //�����Ƿ�Ϊ������
        this->virt = v;
    }

    void reset_virt() {                  // ����Ϊ����ڵ�������ڵ�����Ϊʵ�ʵĽڵ�
        virt = !virt;
    }

    COORD nextxy(short dir) const {              // ��ýڵ�ǰһ���ڵ�����꣬�ɷ��� dir ��ȷ��
        return COORD { cd.X + direct[dir][0], cd.Y + direct[dir][1] };
    }

    void print(DWORD time = info::default_sleep_time) const {         //����Ļ�ϻ滭���ýڵ�
        COORD current_cursor;
        Getxy(current_cursor);              // ��ȡ��ǰ���λ��
        gotoxy(this->cd);
        if(!virt)    printf("%s", snake_char);       // ����������Ļ���ֱ�ӻ�����
        else    printf("%s", space);         // ������������
        Sleep(time);
        gotoxy(current_cursor);            // ���ص�ԭ����λ�ã�Ϊ��ʹ�����������粻����������Ӱ��
    }
};
