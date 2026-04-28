#include "Level3.h"
#include "ui_Level3.h"

Level3::Level3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Level3)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 600, 700);
    view = new QGraphicsView(scene, this);
    view->setGeometry(340, 100, 600, 600);
    view->setBackgroundBrush(QColor("#0F3460"));

    player = new PlayerCar();
    scene->addItem(player);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Level3::updateGame);
    gameTimer->start(16);

    score = 0;
    obstacleTimer = 0;
    enemyTimer = 0;
}

Level3::~Level3()
{
    delete ui;
}

void Level3::keyPressEvent(QKeyEvent *event)
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

void Level3::updateGame()
{
    score++;
    ui->scoreLabel->setText(QString("分数: %1").arg(score));

    obstacleTimer++;
    if (obstacleTimer > 25) {
        int lane = qrand() % 3;
        Obstacle *obstacle = new Obstacle(lane);
        obstacles.append(obstacle);
        scene->addItem(obstacle);
        obstacleTimer = 0;
    }

    enemyTimer++;
    if (enemyTimer > 60) {
        int lane = qrand() % 3;
        Enemy *enemy = new Enemy(lane);
        enemies.append(enemy);
        scene->addItem(enemy);
        enemyTimer = 0;
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

    QList<Enemy*> enemiesToRemove;
    foreach (Enemy *enemy, enemies) {
        enemy->updatePosition();
        if (enemy->isOutOfScreen()) {
            enemiesToRemove.append(enemy);
        }
    }

    foreach (Enemy *enemy, enemiesToRemove) {
        enemies.removeOne(enemy);
        scene->removeItem(enemy);
        delete enemy;
    }

    if (score >= 10000) {
        gameTimer->stop();
        emit levelComplete();
    }
}

void Level3::on_backButton_clicked()
{
    gameTimer->stop();
    emit backToMenu();
}