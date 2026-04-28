#include "Level1.h"
#include "ui_Level1.h"

Level1::Level1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Level1)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 600, 700);
    view = new QGraphicsView(scene, this);
    view->setGeometry(340, 100, 600, 600);
    view->setBackgroundBrush(QColor("#2C3E50"));

    player = new PlayerCar();
    scene->addItem(player);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Level1::updateGame);
    gameTimer->start(16);

    score = 0;
    obstacleTimer = 0;
    enemyTimer = 0;
}

Level1::~Level1()
{
    delete ui;
}

void Level1::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        player->moveLeft();
        break;
    case Qt::Key_Right:
        player->moveRight();
        break;
    case Qt::Key_Up:
        player->moveUp();
        break;
    case Qt::Key_Down:
        player->moveDown();
        break;
    }
}

void Level1::updateGame()
{
    score++;
    ui->scoreLabel->setText(QString("分数: %1").arg(score));

    obstacleTimer++;
    if (obstacleTimer > 60) {
        int lane = qrand() % 3;
        Obstacle *obstacle = new Obstacle(lane);
        obstacles.append(obstacle);
        scene->addItem(obstacle);
        obstacleTimer = 0;
    }

    QList<Obstacle*> obstaclesToRemove;
    foreach (Obstacle *obstacle, obstacles) {
        obstacle->updatePosition();
        if (obstacle->isOutOfScreen()) {
            obstaclesToRemove.append(obstacle);
        }
    }

    foreach (Obstacle *obstacle, obstaclesToRemove) {
        obstacles.removeOne(obstacle);
        scene->removeItem(obstacle);
        delete obstacle;
    }

    if (score >= 5000) {
        gameTimer->stop();
        emit levelComplete();
    }
}

void Level1::on_backButton_clicked()
{
    gameTimer->stop();
    emit backToMenu();
}