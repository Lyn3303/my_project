#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>
#include <QGraphicsObject>
#include <QPixmap>

class Obstacle : public QGraphicsObject
{
    Q_OBJECT

public:
    enum class ObstacleType {
        Type1,
        Type2
    };
    enum class Lane {
        Upper,
        Middle,
        Lower
    };

    explicit Obstacle(ObstacleType type, Lane lane, QObject *parent = nullptr);
    ~Obstacle();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    ObstacleType getType() const;
    Lane getLane() const;
    void moveLeft(qreal speed);
    bool isOffScreen() const;
    void triggerFlash();
    bool isFlashing() const;
    void updateFlash();

private:
    QPixmap obstaclePixmap;
    ObstacleType obstacleType;
    Lane obstacleLane;
    qreal obstacleWidth;
    qreal obstacleHeight;
    bool flashVisible;
    int flashCount;
};

#endif // OBSTACLE_H