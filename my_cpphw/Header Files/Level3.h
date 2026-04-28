#ifndef LEVEL3_H
#define LEVEL3_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include "PlayerCar.h"
#include "Obstacle.h"
#include "Enemy.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Level3; }
QT_END_NAMESPACE

class Level3 : public QWidget
{
    Q_OBJECT

public:
    explicit Level3(QWidget *parent = nullptr);
    ~Level3();

signals:
    void backToMenu();
    void levelComplete();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();
    void on_backButton_clicked();

private:
    Ui::Level3 *ui;
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

#endif // LEVEL3_H