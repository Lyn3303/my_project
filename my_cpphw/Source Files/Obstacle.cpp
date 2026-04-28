#include "Obstacle.h"

Obstacle::Obstacle(int lane, QObject *parent)
    : QObject(parent), m_lane(lane), m_width(60), m_height(60), m_speed(5)
{
    m_x = 200 + lane * 200;
    m_y = -m_height;
}

Obstacle::~Obstacle()
{
}

QRectF Obstacle::boundingRect() const
{
    return QRectF(m_x, m_y, m_width, m_height);
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(QColor("#4ECDC4"));
    painter->drawRect(m_x, m_y, m_width, m_height);

    painter->setBrush(QColor("#2C3E50"));
    painter->drawEllipse(m_x + 10, m_y + 10, 15, 15);
    painter->drawEllipse(m_x + 35, m_y + 10, 15, 15);
    painter->drawEllipse(m_x + 10, m_y + 35, 15, 15);
    painter->drawEllipse(m_x + 35, m_y + 35, 15, 15);
}

void Obstacle::updatePosition()
{
    m_y += m_speed;
    update();
}

bool Obstacle::isOutOfScreen() const
{
    return m_y > 700;
}

int Obstacle::getLane() const
{
    return m_lane;
}