#include "GameWidget.h"
#include <cstdlib>
#include <ctime>
#include <QKeyEvent>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{
    // 初始化随机数种子
    srand(time(nullptr));
    
    // 设置窗口大小
    setFixedSize(800, 600);
    // 设置窗口标题
    setWindowTitle("Spacecraft Simulator");
    
    // 初始化3D物体
    for (int i = 0; i < 100; ++i) {
        Object3D obj;
        obj.x = (rand() % 1000) - 500;
        obj.y = (rand() % 1000) - 500;
        obj.z = rand() % 5000;
        obj.size = 10 + (rand() % 30);
        obj.color = QColor(rand() % 255, rand() % 255, rand() % 255);
        objects.push_back(obj);
    }
    
    // 初始化飞船
    ship.x = 0;
    ship.y = 0;
    ship.z = 0;
    ship.speed = 0;
    ship.state = SHIP_STATE_OFF;
    ship.direction = DIR_FORWARD;
    
    // 初始化相机
    camera.yaw = 0.0f;
    camera.pitch = 0.0f;
    camera.roll = 0.0f;
    camera.distance = 1000.0f;
    
    // 初始化键盘状态
    for (int i = 0; i < 256; ++i) {
        keys[i] = false;
    }
    
    // 创建游戏循环定时器
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameWidget::updateGame);
    gameTimer->start(30); // 约33fps
}

GameWidget::~GameWidget()
{
    delete gameTimer;
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 256) {
        keys[key] = true;
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 256) {
        keys[key] = false;
    }
}

void GameWidget::updateGame()
{
    // 处理键盘输入
    // 速度控制
    if (keys[Qt::Key_W]) {
        // W键：向前加速
        if (keys[Qt::Key_Shift]) {
            // W+Shift：加速（1.5x）
            ship.state = SHIP_STATE_ACCELERATE;
            ship.speed = 15.0f; // 固定速度
        } else {
            // 只按W：正常加速（1x）
            ship.state = SHIP_STATE_FORWARD;
            ship.speed = 10.0f; // 固定速度
        }
    } else if (keys[Qt::Key_R]) {
        // R键：减速（-2x）
        ship.state = SHIP_STATE_DECELERATE;
        ship.speed = 5.0f; // 固定速度
    } else {
        // 没有按W或R：保持当前速度
        ship.state = SHIP_STATE_OFF;
        ship.speed = 0.0f;
    }
    
    // 相机旋转控制
    if (keys[Qt::Key_A]) {
        // A键：其他物体绕z轴右手螺旋定则的反方向旋转
        camera.yaw -= 0.05f;
    } else if (keys[Qt::Key_D]) {
        // D键：其他物体绕z轴右手螺旋定则的正方向旋转
        camera.yaw += 0.05f;
    }
    
    if (keys[Qt::Key_S]) {
        // S键：其他物体绕y轴右手螺旋定则的反方向旋转
        camera.pitch -= 0.05f;
    }
    
    // 移除俯仰角限制，允许任意角度旋转
    
    // 所有物体沿相机视线方向（屏幕外）平移
    for (auto &obj : objects) {
        // 计算物体相对于相机的方向向量
        float dx = obj.x;
        float dy = obj.y;
        float dz = obj.z;
        
        // 应用相机旋转的逆变换，得到物体在相机坐标系中的方向
        // 先绕Y轴旋转的逆变换
        float tempX = dx * cos(camera.pitch) - dz * sin(camera.pitch);
        float tempZ = dx * sin(camera.pitch) + dz * cos(camera.pitch);
        dx = tempX;
        dz = tempZ;
        
        // 再绕Z轴旋转的逆变换
        tempX = dx * cos(camera.yaw) + dy * sin(camera.yaw);
        float tempY = -dx * sin(camera.yaw) + dy * cos(camera.yaw);
        dx = tempX;
        dy = tempY;
        
        // 归一化方向向量
        float length = sqrt(dx*dx + dy*dy + dz*dz);
        if (length > 0) {
            dx /= length;
            dy /= length;
            dz /= length;
        }
        
        // 沿方向向量平移物体
        obj.x -= dx * ship.speed;
        obj.y -= dy * ship.speed;
        obj.z -= dz * ship.speed;
        
        // 当物体移出视野时，重置到屏幕外前方
        if (obj.x < -1000 || obj.y < -2000 || obj.y > 2000 || obj.z < -2000 || obj.z > 2000) {
            obj.x = 3000 + (rand() % 2000);
            obj.y = (rand() % 4000) - 2000;
            obj.z = (rand() % 4000) - 2000;
        }
    }
    
    // 触发重绘
    update();
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 清空画布
    painter.fillRect(rect(), Qt::black);
    
    // 焦距
    const float focalLength = 500.0f;
    
    // 深度排序
    std::sort(objects.begin(), objects.end(), [](const Object3D &a, const Object3D &b) {
        // 基于物体的x坐标进行排序，x值越大的物体离屏幕越远
        return a.x > b.x;
    });
    
    // 绘制所有物体
    for (const auto &obj : objects) {
        // 计算物体相对于屏幕中心的坐标
        float relX = obj.x;
        float relY = obj.y;
        float relZ = obj.z;
        
        // 应用相机旋转
        // 先绕Z轴旋转（A/D键控制）
        float tempX = relX * cos(camera.yaw) - relY * sin(camera.yaw);
        float tempY = relX * sin(camera.yaw) + relY * cos(camera.yaw);
        relX = tempX;
        relY = tempY;
        
        // 再绕Y轴旋转（S键控制）
        tempX = relX * cos(camera.pitch) + relZ * sin(camera.pitch);
        float tempZ = -relX * sin(camera.pitch) + relZ * cos(camera.pitch);
        relX = tempX;
        relZ = tempZ;
        
        // 跳过太近或太远的物体
        if (relX < 10 || relX > 5000) continue;
        
        // 计算屏幕坐标和大小
        float scale = focalLength / relX;
        int screenX = width() / 2 + relY * scale;
        int screenY = height() / 2 + relZ * scale;
        int screenSize = obj.size * scale;
        
        // 绘制物体
        painter.setBrush(obj.color);
        painter.drawEllipse(screenX - screenSize/2, screenY - screenSize/2, screenSize, screenSize);
    }
    
    // 绘制飞船（始终在屏幕中心）
    int shipScreenX = width() / 2;
    int shipScreenY = height() / 2;
    int shipScreenSize = 50;
    
    // 绘制飞船主体
    painter.setBrush(Qt::white);
    painter.drawEllipse(shipScreenX - shipScreenSize/2, shipScreenY - shipScreenSize/2, shipScreenSize, shipScreenSize);
    
    // 绘制尾焰
    if (ship.state != SHIP_STATE_OFF) {
        int flameSize = shipScreenSize / 2;
        if (ship.state == SHIP_STATE_ACCELERATE) {
            // 加速状态：红色大尾焰
            painter.setBrush(Qt::red);
            flameSize = shipScreenSize;
        } else if (ship.state == SHIP_STATE_FORWARD) {
            // 向前状态：黄色小尾焰
            painter.setBrush(Qt::yellow);
            flameSize = shipScreenSize / 2;
        } else if (ship.state == SHIP_STATE_DECELERATE) {
            // 减速状态：蓝色尾焰
            painter.setBrush(Qt::blue);
            flameSize = shipScreenSize / 2;
        }
        
        // 绘制尾焰在飞船前方（x轴负方向）
        painter.drawEllipse(shipScreenX - flameSize/2, shipScreenY - shipScreenSize/2 - flameSize, flameSize, flameSize);
    }
}