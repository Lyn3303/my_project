#ifndef PLAYERCAR_H
#define PLAYERCAR_H

#include <QObject>
#include <QGraphicsObject>
#include <QTimer>
#include <QPixmap>

class PlayerCar : public QGraphicsObject
{
    Q_OBJECT

public:
    enum class State {
        Normal,
        Crash,
        Shift
    };
    enum class Position {
        Upper,
        Middle,
        Lower
    };

    explicit PlayerCar(QObject *parent = nullptr);
    ~PlayerCar();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setState(State state);
    State getState() const;
    Position getPosition() const;
    bool isShifting() const;

public slots:
    void triggerCrash();
    void endCrash();
    void moveUp();
    void moveDown();
    void pressShift();
    void releaseShift();

private:
    QPixmap carPixmap;
    State currentState;
    Position currentPosition;
    qreal baseWidth;
    qreal baseHeight;
    QTimer *crashTimer;
    QTimer *shiftFlashTimer;
    QTimer *moveTimer;
    QTimer *shakeTimer;
    bool crashVisible;
    int crashFlashCount;
    bool isShiftPressed;
    qreal shiftFlashAlpha;
    bool shiftFlashFadeIn;
    bool isMoving;
    qreal moveSpeed;
    qreal targetY;
    qreal shakeOffsetX;
    qreal shakeOffsetY;
    int trailLength;
    QList<QPointF> trailPositions;

    void updatePosition();
    void startSmoothMove();
    void moveStep();
};

#endif // PLAYERCAR_H