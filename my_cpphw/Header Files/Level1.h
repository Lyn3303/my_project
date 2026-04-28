#ifndef LEVEL1_H
#define LEVEL1_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include "PlayerCar.h"
#include "Obstacle.h"
#include "Enemy.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Level1; }
QT_END_NAMESPACE

class Level1 : public QWidget
{
    Q_OBJECT

public:
    explicit Level1(QWidget *parent = nullptr);
    ~Level1();

signals:
    void backToMenu();
    void levelComplete();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();
    void on_backButton_clicked();

private:
    Ui::Level1 *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    PlayerCar *player;
    QList<Obstacle*> obstacles;
    QList<Enemy*> enemies;
    QTimer *gameTimer;
    int score;
    int obstacleTimer;
    int enemyTimer;
};

#endif // LEVEL1_H