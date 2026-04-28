#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QGraphicsItem>

class Enemy : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit Enemy(int lane, QObject *parent = nullptr);
    ~Enemy();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void updatePosition();
    bool isOutOfScreen() const;
    int getLane() const;
    void setLane(int lane);

private:
    qreal m_x;
    qreal m_y;
    qreal m_width;
    qreal m_height;
    int m_lane;
    int m_speed;
};

#endif // ENEMY_H