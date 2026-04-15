#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <vector>

struct Object3D
{
    float x, y, z;
    float size;
    QColor color;
};

enum ShipState {
    SHIP_STATE_OFF,     // 熄火
    SHIP_STATE_FORWARD,  // 向前
    SHIP_STATE_ACCELERATE, // 加速
    SHIP_STATE_DECELERATE // 减速
};

enum ShipDirection {
    DIR_FORWARD,   // 正向
    DIR_DOWN,      // 斜向下
    DIR_LEFT,      // 斜向左
    DIR_RIGHT      // 斜向右
};

struct Spaceship {
    float x, y, z;
    float speed;
    ShipState state;
    ShipDirection direction;
};

struct Camera {
    float yaw;   // 偏航角（绕Z轴旋转）
    float pitch; // 俯仰角（绕Y轴旋转）
    float roll;  // 翻滚角（绕X轴旋转）
    float distance; // 相机到飞船的距离
};

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

private:
    std::vector<Object3D> objects;
    QTimer *gameTimer;
    Spaceship ship;
    Camera camera;
    bool keys[256]; // 用于存储键盘状态
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private slots:
    void updateGame();
};

#endif // GAMEWIDGET_H