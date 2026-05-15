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

    shiftFlashTimer = new QTimer(this);  // 切换状态定时器
    shiftFlashTimer->setInterval(50);
    connect(shiftFlashTimer, &QTimer::timeout, [this]() {  
        trailPositions.append(QPointF(0, shakeOffsetY));
        if (trailPositions.size() > trailLength) {
            trailPositions.removeFirst();
        }
        update();
    });

    moveTimer = new QTimer(this);  // 玩家移动定时器
    moveTimer->setInterval(16);
    connect(moveTimer, &QTimer::timeout, this, &PlayerCar::moveStep);

    shakeTimer = new QTimer(this);  // 玩家震动效果显示
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

    if (currentState == State::Shift) {      // 加速状态下的车辆拖尾效果
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

PlayerCar::State PlayerCar::getState() const  //获取玩家状态
{
    return currentState;
}

PlayerCar::Position PlayerCar::getPosition() const  //获取玩家位置
{
    return currentPosition;
}

bool PlayerCar::isShifting() const
{
    return isShiftPressed;
}

void PlayerCar::triggerCrash()  //触发碰撞状态
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

void PlayerCar::endCrash() // 碰撞状态结束
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

void PlayerCar::moveUp() // 上移动
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

void PlayerCar::moveDown() // 下移动
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

void PlayerCar::startSmoothMove() // 平滑移动
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

void PlayerCar::moveStep()  //步进函数
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

void PlayerCar::pressShift() //转换为加速状态
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

void PlayerCar::releaseShift() // 加速结束
{
    if (!isShiftPressed) {
        return;
    }
    isShiftPressed = false;
    shiftFlashTimer->stop();
    currentState = State::Normal;
    update();
}

void PlayerCar::updatePosition() // 更新位置
{
    qreal upperY = 305;
    qreal middleY = 440;
    qreal lowerY = 575;
    qreal y = middleY;
    switch (currentPosition) 
    {
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