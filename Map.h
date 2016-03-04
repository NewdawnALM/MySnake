#include "Node.h"
#include <vector>
using std::vector;

using namespace info;

class _map {

public:
    short map[303][303];
    const short barrier = 1;        // �ϰ���
    const short space = 0;          // ���н��
    const short candy = 2;          // �ǹ�

    vector<node> wall_node;

    _map() {
        memset(map, 0, sizeof map);
        wall_node.clear();
    }

    // ���һ��ǽ���
    void clearWallNode(node &_wall) {
        setMap(_wall.getxy(), space, _wall.length());
        _wall.setvirt(true);
        _wall.print(0);
    }

    // �������ǽ��
    void clearAllWall() {
        for_each(wall_node.begin(), wall_node.end(), [&](node &n) {  clearWallNode(n);  }) ;
        wall_node.clear();
    }

    // ������ͼ�ұߺ͵ײ��ı߽�
    void paintBorder() {

        // ������ͼ�ĵײ��߽磬���ƶ������½�
        COORD cd{border_left, border_down};
        gotoxy(cd);
        while( cd.X <= border_right ) {
            setMap(cd, barrier, step);      // ͬʱ���ñ߽�Ϊ�ϰ���
            printf("%s", board_bottom);
            Getxy(cd);
        }

        // ������ͼ���ұ߽߱磬���ƶ������Ͻ�
        cd.X = border_right;
        cd.Y = border_top;
        gotoxy(cd);
        while( cd.Y <= border_down ) {
            setMap(cd, barrier, step);
            printf("%s", board_side);
            ++cd.Y;                 // ��Ϊ���ÿ����һ�����ֻ�������ƣ�������Ҫ�ֶ��ѹ��������
            gotoxy(cd);
        }
    }

    // ���õ�ͼ��Ӧ����״ֵ̬��len Ϊ����Ӧ�ĳ���
    void setMap(SHORT x, SHORT y, short status, short len) {
        for(int i = 0; i < len; ++i)
            map[y][x+i] = status;
    }

    void setMap(COORD cd, short status, short len) {
        setMap(cd.X, cd.Y, status, len);
    }

    // �ж��Ƿ��ڵ�ͼ�߽��ڲ�
    bool inBorder(COORD cd) const {
        return !outOfBorder(cd);
    }

    // �ж��Ƿ񳬳���ͼ�߽磨ע�⣬�����Ϊ�˷��㣬�ѵ�ͼ���Ͽ����±գ�Ҳ����˵���±߽綼Ϊ�ϰ�������ϱ߽�Ϊ���н�㣩
    bool outOfBorder(COORD cd) const {
        return cd.X < border_left || cd.X >= border_right || cd.Y < border_top || cd.Y >= border_down;
    }

    // �ж��Ƿ�Ϊ�ϰ���
    bool isBarrier(COORD cd) const {
        return map[cd.Y][cd.X] == barrier;
    }

    // �ж��Ƿ�����ײ���������ϰ�����߳����߽綼������
    bool isCrash(COORD cd) const {
        return isBarrier(cd) || outOfBorder(cd);
    }

    // �жϸý���Ƿ�Ϊ�ǹ�
    bool isCandy(COORD cd) const {
        return map[cd.Y][cd.X] == candy;
    }
};
