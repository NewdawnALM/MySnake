#include "Node.h"
#include <vector>
using std::vector;

using namespace info;

class _map {

public:
    short map[303][303];
    const short barrier = 1;        // 障碍物
    const short space = 0;          // 可行结点
    const short candy = 2;          // 糖果

    vector<node> wall_node;

    _map() {
        memset(map, 0, sizeof map);
        wall_node.clear();
    }

    // 清除一个墙结点
    void clearWallNode(node &_wall) {
        setMap(_wall.getxy(), space, _wall.length());
        _wall.setvirt(true);
        _wall.print(0);
    }

    // 清除所有墙壁
    void clearAllWall() {
        for_each(wall_node.begin(), wall_node.end(), [&](node &n) {  clearWallNode(n);  }) ;
        wall_node.clear();
    }

    // 画出地图右边和底部的边界
    void paintBorder() {

        // 画出地图的底部边界，先移动到左下角
        COORD cd{border_left, border_down};
        gotoxy(cd);
        while( cd.X <= border_right ) {
            setMap(cd, barrier, step);      // 同时设置边界为障碍物
            printf("%s", board_bottom);
            Getxy(cd);
        }

        // 画出地图的右边边界，先移动到右上角
        cd.X = border_right;
        cd.Y = border_top;
        gotoxy(cd);
        while( cd.Y <= border_down ) {
            setMap(cd, barrier, step);
            printf("%s", board_side);
            ++cd.Y;                 // 因为光标每画完一个结点只会向右移，所以需要手动把光标向下移
            gotoxy(cd);
        }
    }

    // 设置地图对应结点的状态值，len 为结点对应的长度
    void setMap(SHORT x, SHORT y, short status, short len) {
        for(int i = 0; i < len; ++i)
            map[y][x+i] = status;
    }

    void setMap(COORD cd, short status, short len) {
        setMap(cd.X, cd.Y, status, len);
    }

    // 判断是否在地图边界内部
    bool inBorder(COORD cd) const {
        return !outOfBorder(cd);
    }

    // 判断是否超出地图边界（注意，在这里，为了方便，把地图左上开右下闭，也就是说右下边界都为障碍物，而左上边界为可行结点）
    bool outOfBorder(COORD cd) const {
        return cd.X < border_left || cd.X >= border_right || cd.Y < border_top || cd.Y >= border_down;
    }

    // 判断是否为障碍物
    bool isBarrier(COORD cd) const {
        return map[cd.Y][cd.X] == barrier;
    }

    // 判断是否发生碰撞，即碰到障碍物或者超出边界都返回真
    bool isCrash(COORD cd) const {
        return isBarrier(cd) || outOfBorder(cd);
    }

    // 判断该结点是否为糖果
    bool isCandy(COORD cd) const {
        return map[cd.Y][cd.X] == candy;
    }
};
