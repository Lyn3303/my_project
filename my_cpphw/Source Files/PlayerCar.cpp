#include "PlayerCar.h"
#include <QPainter>
#include <QDebug>

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
        if (shiftFlashFadeIn) {
            shiftFlashAlpha += 0.06;
            if (shiftFlashAlpha >= 0.45) {
                shiftFlashAlpha = 0.45;
                shiftFlashFadeIn = false;
            }
        } else {
            shiftFlashAlpha -= 0.06;
            if (shiftFlashAlpha <= 0.15) {
                shiftFlashAlpha = 0.15;
                shiftFlashFadeIn = true;
            }
        }
        update();
    });

    moveTimer = new QTimer(this);
    moveTimer->setInterval(16);
    connect(moveTimer, &QTimer::timeout, this, &PlayerCar::moveStep);

    setPos(256, 440);
    setZValue(1000);
}

PlayerCar::~PlayerCar()
{
    delete crashTimer;
    delete shiftFlashTimer;
    delete moveTimer;
}

QRectF PlayerCar::boundingRect() const
{
    return QRectF(0, 0, baseWidth, baseHeight);
}

void PlayerCar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(0, 0, carPixmap);

    if (currentState == State::Shift) {
        painter->fillRect(0, 0, carPixmap.width(), carPixmap.height(), QColor(135, 206, 250, static_cast<int>(shiftFlashAlpha * 255)));
    } else if (currentState == State::Crash) {
        if (!crashVisible) {
            painter->fillRect(0, 0, carPixmap.width(), carPixmap.height(), QColor(255, 0, 0, 100));
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