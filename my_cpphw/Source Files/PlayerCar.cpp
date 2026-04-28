#include "PlayerCar.h"

PlayerCar::PlayerCar(QObject *parent)
    : QObject(parent), m_x(400), m_y(500), m_width(60), m_height(100), m_lane(1)
{
}

PlayerCar::~PlayerCar()
{
}

QRectF PlayerCar::boundingRect() const
{
    return QRectF(m_x, m_y, m_width, m_height);
}

void PlayerCar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(QColor("#FF6B6B"));
    painter->drawRect(m_x, m_y, m_width, m_height);

    painter->setBrush(QColor("#FFFFFF"));
    painter->drawRect(m_x + 5, m_y + 10, 20, 20);
    painter->drawRect(m_x + 35, m_y + 10, 20, 20);
    painter->drawRect(m_x + 5, m_y + 70, 20, 20);
    painter->drawRect(m_x + 35, m_y + 70, 20, 20);
}

void PlayerCar::moveLeft()
{
    if (m_lane > 0) {
        m_lane--;
        m_x = 200 + m_lane * 200;
        update();
    }
}

void PlayerCar::moveRight()
{
    if (m_lane < 2) {
        m_lane++;
        m_x = 200 + m_lane * 200;
        update();
    }
}

void PlayerCar::moveUp()
{
    if (m_y > 50) {
        m_y -= 30;
        update();
    }
}

void PlayerCar::moveDown()
{
    if (m_y < 550) {
        m_y += 30;
        update();
    }
}