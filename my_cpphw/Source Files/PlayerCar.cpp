#include "PlayerCar.h"
#include <QPainter>
#include <QDebug>
#include <QRandomGenerator>

PlayerCar::PlayerCar(QObject *parent)
    : QGraphicsObject(nullptr)
    , currentState(State::Normal)
    , currentPosition(Position::Middle)
    , baseWidth(300)
    , baseHeight(400)
    , crashVisible(true)
    , crashFlashCount(0)
    , isShiftPressed(false)
    , shiftFlashAlpha(0.3)
    , shiftFlashFadeIn(true)
    , isMoving(false)
    , moveSpeed(15.0)
    , shakeOffsetX(0)
    , shakeOffsetY(0)
    , trailLength(8)
{
    Q_UNUSED(parent);

    carPixmap = QPixmap(":/content/Car.PNG");
    if (carPixmap.isNull()) {
        qDebug() << "Car pixmap load failed!";
    } else {
        carPixmap = carPixmap.scaled(baseWidth, baseHeight, Qt::KeepAspectRatio);
    }

    crashTimer = new QTimer(this);
    connect(crashTimer, &QTimer::timeout, this, &PlayerCar::endCrash);

    shiftFlashTimer = new QTimer(this);
    shiftFlashTimer->setInterval(50);
    connect(shiftFlashTimer, &QTimer::timeout, [this]() {
        trailPositions.append(QPointF(0, shakeOffsetY));
        if (trailPositions.size() > trailLength) {
            trailPositions.removeFirst();
        }
        update();
    });

    moveTimer = new QTimer(this);
    moveTimer->setInterval(16);
    connect(moveTimer, &QTimer::timeout, this, &PlayerCar::moveStep);

    shakeTimer = new QTimer(this);
    shakeTimer->setInterval(16);
    connect(shakeTimer, &QTimer::timeout, [this]() {
        shakeOffsetY = (QRandomGenerator::global()->bounded(-2, 3)) * 0.8;
        update();
    });
    shakeTimer->start();

    setPos(256, 440);
    setZValue(1000);
}

PlayerCar::~PlayerCar()
{
    delete crashTimer;
    delete shiftFlashTimer;
    delete moveTimer;
    delete shakeTimer;
}

QRectF PlayerCar::boundingRect() const
{
    return QRectF(0, 0, baseWidth, baseHeight);
}

void PlayerCar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (currentState == State::Shift) {
        int totalTrails = trailPositions.size();
        for (int i = totalTrails - 1; i >= 0; --i) {
            int distanceIndex = totalTrails - 1 - i;
            qreal progress = static_cast<qreal>(distanceIndex) / (totalTrails - 1);
            qreal alpha = 0.43 - (progress * progress * 0.4);
            qreal offsetX = -(distanceIndex + 1) * 35;
            painter->setOpacity(alpha);
            painter->drawPixmap(shakeOffsetX + offsetX, trailPositions[i].y(), carPixmap);
        }
        painter->setOpacity(1.0);
    }

    painter->drawPixmap(shakeOffsetX, shakeOffsetY, carPixmap);

    if (currentState == State::Crash) {
        if (!crashVisible) {
            painter->fillRect(shakeOffsetX, shakeOffsetY, carPixmap.width(), carPixmap.height(), QColor(255, 0, 0, 100));
        }
    }
}

void PlayerCar::setState(State state)
{
    currentState = state;
    update();
}

PlayerCar::State PlayerCar::getState() const
{
    return currentState;
}

PlayerCar::Position PlayerCar::getPosition() const
{
    return currentPosition;
}

void PlayerCar::triggerCrash()
{
    currentState = State::Crash;
    crashVisible = true;
    crashFlashCount = 0;
    crashTimer->start(100);
    if (shiftFlashTimer->isActive()) {
        shiftFlashTimer->stop();
    }
    update();
}

void PlayerCar::endCrash()
{
    crashFlashCount++;
    crashVisible = !crashVisible;
    update();

    if (crashFlashCount >= 10) {
        crashTimer->stop();
        currentState = State::Normal;
        crashVisible = true;
        update();
    }
}

void PlayerCar::moveUp()
{
    if (isMoving) {
        return;
    }
    switch (currentPosition) {
    case Position::Middle:
        currentPosition = Position::Upper;
        break;
    case Position::Lower:
        currentPosition = Position::Middle;
        break;
    case Position::Upper:
        return;
    }
    startSmoothMove();
}

void PlayerCar::moveDown()
{
    if (isMoving) {
        return;
    }
    switch (currentPosition) {
    case Position::Middle:
        currentPosition = Position::Lower;
        break;
    case Position::Upper:
        currentPosition = Position::Middle;
        break;
    case Position::Lower:
        return;
    }
    startSmoothMove();
}

void PlayerCar::startSmoothMove()
{
    qreal upperY = 305;
    qreal middleY = 440;
    qreal lowerY = 575;

    targetY = middleY;
    switch (currentPosition) {
    case Position::Upper:
        targetY = upperY;
        break;
    case Position::Middle:
        targetY = middleY;
        break;
    case Position::Lower:
        targetY = lowerY;
        break;
    }

    isMoving = true;
    moveTimer->start();
}

void PlayerCar::moveStep()
{
    qreal currentY = pos().y();
    qreal dy = targetY - currentY;

    if (qAbs(dy) < moveSpeed) {
        setPos(256, targetY);
        isMoving = false;
        moveTimer->stop();
    } else {
        setPos(256, currentY + (dy > 0 ? moveSpeed : -moveSpeed));
    }
    update();
}

void PlayerCar::pressShift()
{
    if (currentState == State::Crash || isShiftPressed) {
        return;
    }
    isShiftPressed = true;
    currentState = State::Shift;
    shiftFlashAlpha = 0.3;
    shiftFlashFadeIn = true;
    shiftFlashTimer->start();
    trailPositions.clear();
}

void PlayerCar::releaseShift()
{
    if (!isShiftPressed) {
        return;
    }
    isShiftPressed = false;
    shiftFlashTimer->stop();
    currentState = State::Normal;
    update();
}

void PlayerCar::updatePosition()
{
    qreal upperY = 305;
    qreal middleY = 440;
    qreal lowerY = 575;

    qreal y = middleY;
    switch (currentPosition) {
    case Position::Upper:
        y = upperY;
        break;
    case Position::Middle:
        y = middleY;
        break;
    case Position::Lower:
        y = lowerY;
        break;
    }

    setPos(256, y);
}