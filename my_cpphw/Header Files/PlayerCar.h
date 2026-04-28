#ifndef PLAYERCAR_H
#define PLAYERCAR_H

#include <QObject>
#include <QGraphicsItem>
#include <QKeyEvent>

class PlayerCar : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit PlayerCar(QObject *parent = nullptr);
    ~PlayerCar();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

private:
    qreal m_x;
    qreal m_y;
    qreal m_width;
    qreal m_height;
    int m_lane;
};

#endif // PLAYERCAR_H