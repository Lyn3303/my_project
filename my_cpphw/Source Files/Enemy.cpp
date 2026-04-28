#include "Enemy.h"

Enemy::Enemy(int lane, QObject *parent)
    : QObject(parent), m_lane(lane), m_width(60), m_height(100), m_speed(3)
{
    m_x = 200 + lane * 200;
    m_y = -m_height;
}

Enemy::~Enemy()
{
}

QRectF Enemy::boundingRect() const
{
    return QRectF(m_x, m_y, m_width, m_height);
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(QColor("#9B59B6"));
    painter->drawRect(m_x, m_y, m_width, m_height);

    painter->setBrush(QColor("#FFFFFF"));
    painter->drawRect(m_x + 5, m_y + 10, 20, 20);
    painter->drawRect(m_x + 35, m_y + 10, 20, 20);
    painter->drawRect(m_x + 5, m_y + 70, 20, 20);
    painter->drawRect(m_x + 35, m_y + 70, 20, 20);

    painter->setBrush(QColor("#E74C3C"));
    painter->drawRect(m_x + 10, m_y + 40, 40, 20);
}

void Enemy::updatePosition()
{
    m_y += m_speed;
    update();
}

bool Enemy::isOutOfScreen() const
{
    return m_y > 700;
}

int Enemy::getLane() const
{
    return m_lane;
}

void Enemy::setLane(int lane)
{
    m_lane = lane;
    m_x = 200 + lane * 200;
    update();
}