#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>
#include <QGraphicsItem>

class Obstacle : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit Obstacle(int lane, QObject *parent = nullptr);
    ~Obstacle();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void updatePosition();
    bool isOutOfScreen() const;
    int getLane() const;

private:
    qreal m_x;
    qreal m_y;
    qreal m_width;
    qreal m_height;
    int m_lane;
    int m_speed;
};

#endif // OBSTACLE_H