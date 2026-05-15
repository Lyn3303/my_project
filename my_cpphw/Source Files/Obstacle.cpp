#include "Obstacle.h"
#include <QPainter>
#include <QDebug>

Obstacle::Obstacle(ObstacleType type, Lane lane, QObject *parent)
    : QGraphicsObject(nullptr)
    , obstacleType(type)
    , obstacleLane(lane)
    , flashVisible(true)
    , flashCount(0)
{
    Q_UNUSED(parent);

    QString pixmapPath;
    if (type == ObstacleType::Type1) {
        pixmapPath = ":/content/ob1.PNG";
        obstacleWidth = 30;
        obstacleHeight = 100;
    } else {
        pixmapPath = ":/content/ob2.PNG";
        obstacleWidth = 40;
        obstacleHeight = 125;
    }

    obstaclePixmap = QPixmap(pixmapPath);
    qDebug() << "Loading obstacle:" << pixmapPath << "type:" << static_cast<int>(type);
    if (obstaclePixmap.isNull()) {
        qDebug() << "Obstacle pixmap load failed:" << pixmapPath;
    } else {
        qDebug() << "Original size:" << obstaclePixmap.size();
        obstaclePixmap = obstaclePixmap.scaled(obstacleWidth, obstacleHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        qDebug() << "Scaled size:" << obstaclePixmap.size();
    }

    qreal startY;
    switch (lane) {
    case Lane::Upper:
        startY = 355;
        break;
    case Lane::Middle:
        startY = 490;
        break;
    case Lane::Lower:
        startY = 625;
        break;
    }

    setPos(1280, startY);
    setZValue(800);
}

QRectF Obstacle::boundingRect() const  //设置碰撞矩形
{
    return QRectF(0, 0, obstaclePixmap.width(), obstaclePixmap.height());  
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)  //绘制障碍物
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(0, 0, obstaclePixmap);
    if (flashCount > 0 && !flashVisible) {
        painter->fillRect(0, 0, obstaclePixmap.width(), obstaclePixmap.height(), QColor(255, 0, 0, 150));
    }
}

Obstacle::ObstacleType Obstacle::getType() const  //获取障碍物类型
{
    return obstacleType;
}

Obstacle::Lane Obstacle::getLane() const  //障碍物车道
{
    return obstacleLane;
}

void Obstacle::moveLeft(qreal speed)  //障碍物左移
{
    setPos(pos().x() - speed, pos().y());
}

bool Obstacle::isOffScreen() const  //障碍物越界判断
{
    return pos().x() + obstaclePixmap.width() < 0;
}

void Obstacle::triggerFlash()  //障碍物闪烁
{
    flashVisible = false;
    flashCount = 20;
    update();
}

bool Obstacle::isFlashing() const
{
    return flashCount > 0;
}

void Obstacle::updateFlash()  //更新障碍物
{
    if (flashCount > 0) {
        flashCount--;
        flashVisible = !flashVisible;
        update();
        if (flashCount <= 0) {
            flashVisible = true;
            update();
        }
    }
}