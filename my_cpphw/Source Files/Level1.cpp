#include "Level1.h"
#include "mainwindow.h"
#include "LevelSelect.h"
#include "PlayerCar.h"
#include "Obstacle.h"

#include <QKeyEvent>
#include <QPixmap>
#include <QShowEvent>
#include <QDebug>
#include <QLabel>
#include <QGraphicsView>
#include <QMessageBox>
#include <QRandomGenerator>
#include <cmath>

Level1::Level1(QWidget *parent)
    : QWidget(parent)
    , streetOffset(0)
    , bgOffset(0)
    , crashCount(0)
    , street1(nullptr)
    , street2(nullptr)
    , bg1(nullptr)
    , bg2(nullptr)
    , gameTimer(nullptr)
    , bgTimer(nullptr)
    , obstacleSpawnTimer(nullptr)
    , scoreLabel(nullptr)
    , mainWindow(qobject_cast<MainWindow*>(parent))
    , scene(nullptr)
    , graphicsView(nullptr)
    , playerCar(nullptr)
    , isGameOver(false)
    , obstacleSpawnInterval(1000)
    , lastObstacleType(0)
    , windPlayTime(0)
    , bpmAccumulator(0)
    , bpm47Accumulator(0)
    , obstacleFlashTimer(nullptr)
    , gameElapsedTime(0)
{
    setFixedSize(1280, 720);
    setStyleSheet("background-color: black;"); //背景颜色设置

    l1Player = new QMediaPlayer(this);          //L1音乐播放器
    l1AudioOutput = new QAudioOutput(this);
    l1Player->setAudioOutput(l1AudioOutput);
    l1AudioOutput->setVolume(0.8);
    connect(l1Player, &QMediaPlayer::playbackStateChanged, this, &Level1::onL1Finished);

    windPlayer = new QMediaPlayer(this);
    windAudioOutput = new QAudioOutput(this);
    windPlayer->setAudioOutput(windAudioOutput);
    windAudioOutput->setVolume(0.1);
    windPlayer->setLoops(QMediaPlayer::Infinite);

    shiftPlayer = new QMediaPlayer(this);      //加速音乐播放器
    shiftAudioOutput = new QAudioOutput(this);
    shiftPlayer->setAudioOutput(shiftAudioOutput);
    shiftAudioOutput->setVolume(0.3);
    shiftPlayer->setLoops(QMediaPlayer::Infinite);
    connect(shiftPlayer, &QMediaPlayer::playbackStateChanged, this, &Level1::onShiftFinished);

    crashPlayer = new QMediaPlayer(this);      //碰撞音播放器
    crashAudioOutput = new QAudioOutput(this);
    crashPlayer->setAudioOutput(crashAudioOutput);
    crashAudioOutput->setVolume(0.6);

    scene = new QGraphicsScene(this);         
       scene->setSceneRect(0, 0, 1280, 720);
    scene->setBackgroundBrush(Qt::NoBrush);

    QGraphicsView *view = new QGraphicsView(scene, this);  //场景设置
    view->setGeometry(0, 0, 1280, 720);
    view->setStyleSheet("background: transparent; border: none;");
    view->setAttribute(Qt::WA_TranslucentBackground);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setForegroundBrush(Qt::NoBrush);
    view->viewport()->setStyleSheet("background: transparent;");
    view->viewport()->setFocusPolicy(Qt::StrongFocus);
    view->setFocusPolicy(Qt::NoFocus);
    view->viewport()->installEventFilter(this);
    graphicsView = view;

    playerCar = new PlayerCar(this);  
    scene->addItem(playerCar);

    scoreLabel = new QLabel(this);        //分数记录
    scoreLabel->setGeometry(20, 20, 200, 30);
    scoreLabel->setStyleSheet("font-size: 20px; color: rgb(255, 248, 32); background: transparent;");
    scoreLabel->setText("Crash：0");

    qDebug() << "Level1 constructed";
}

void Level1::startLevelMusic()    //总音乐播放管理器
{
    l1Player->setSource(QUrl("qrc:/music/L1.mp3"));
    windPlayer->setSource(QUrl("qrc:/sound/wind.mp3"));
    shiftPlayer->setSource(QUrl("qrc:/sound/shift.mp3"));
    crashPlayer->setSource(QUrl("qrc:/sound/crash.mp3"));
    l1Player->play();
    windPlayer->play();

    obstacleSpawnTimer = new QTimer(this);   //障碍物生成定时器
    connect(obstacleSpawnTimer, &QTimer::timeout, this, &Level1::spawnObstacle);
    obstacleSpawnTimer->start(obstacleSpawnInterval);

    obstacleFlashTimer = new QTimer(this);  
    obstacleFlashTimer->setInterval(50);
    connect(obstacleFlashTimer, &QTimer::timeout, this, [this]() {    //障碍物闪烁定时器
        for (Obstacle *ob : obstacles) {
            if (ob->isFlashing()) {
                ob->updateFlash();
            }
        }
    });
    obstacleFlashTimer->start();
}

void Level1::stopLevelMusic()    //停止音乐
{
    l1Player->stop();
    windPlayer->stop();
    shiftPlayer->stop();
    if (obstacleSpawnTimer) {
        obstacleSpawnTimer->stop();
    }
    if (obstacleFlashTimer) {
        obstacleFlashTimer->stop();
    }
}

void Level1::showEvent(QShowEvent *event)      
{
    QWidget::showEvent(event);
    qDebug() << "Level1 shown, size:" << size();

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    if (street1 == nullptr && street2 == nullptr) {
        QPixmap streetPixmap(":/content/street.PNG");
        qDebug() << "Street pixmap loaded:" << !streetPixmap.isNull();
        if (!streetPixmap.isNull()) {
            scaledStreetPixmap = streetPixmap.scaledToWidth(1280);
            streetHeight = scaledStreetPixmap.height();
            streetWidth = scaledStreetPixmap.width();

            street1 = new QLabel(this);
            street1->setPixmap(scaledStreetPixmap);
            street1->setGeometry(0, height() - streetHeight, streetWidth, streetHeight);
            street1->setAttribute(Qt::WA_TranslucentBackground);
            street1->setStyleSheet("background: transparent;");
            street1->show();

            street2 = new QLabel(this);
            street2->setPixmap(scaledStreetPixmap);
            street2->setGeometry(streetWidth, height() - streetHeight, streetWidth, streetHeight);
            street2->setAttribute(Qt::WA_TranslucentBackground);
            street2->setStyleSheet("background: transparent;");
            street2->show();
        }

        gameTimer = new QTimer(this);
        connect(gameTimer, &QTimer::timeout, this, &Level1::scrollStreet);
        connect(gameTimer, &QTimer::timeout, this, &Level1::updateGame);
        connect(gameTimer, &QTimer::timeout, this, &Level1::updateObstacles);
        connect(gameTimer, &QTimer::timeout, this, &Level1::checkCollisions);
        gameTimer->start(30);
    }

    if (bg1 == nullptr && bg2 == nullptr) {  //背景设置
        QPixmap bgPixmap(":/bk/L1.png");
        qDebug() << "BG L1 loaded:" << !bgPixmap.isNull();

        bgWidth = bgPixmap.width() * 0.8;
        bgHeight = bgPixmap.height() * 0.8;
        QPixmap scaledBg = bgPixmap.scaled(bgWidth, bgHeight, Qt::KeepAspectRatio);

        bg1 = new QLabel(this);
        bg1->setPixmap(scaledBg);
        bg1->setGeometry(0, -220, bgWidth, bgHeight);
        bg1->show();
        bg1->lower();

        bg2 = new QLabel(this);   //背景2衔接背景1，实现循环滚动效果
        bg2->setPixmap(scaledBg);
        bg2->setGeometry(bgWidth, -220, bgWidth, bgHeight);
        bg2->show();
        bg2->lower();
        bgTimer = new QTimer(this);  
        connect(bgTimer, &QTimer::timeout, this, &Level1::scrollBackground);
        bgTimer->start(200);
    }
    if (scoreLabel) {
        scoreLabel->raise();    
    }
    if (graphicsView) {
        graphicsView->raise();
    }
    if (playerCar) {
        playerCar->setZValue(1000);
    }
    startLevelMusic();
}

Level1::~Level1()
{
    stopLevelMusic();
    clearObstacles();
}

void Level1::keyPressEvent(QKeyEvent *event)  
{
    if (isGameOver) {
        return;
    }

    if (event->key() == Qt::Key_Escape) {       //esc响应退出游戏
        qDebug() << "Escape pressed in Level1";
        if (gameTimer) gameTimer->stop();
        if (bgTimer) bgTimer->stop();
        stopLevelMusic();
        clearObstacles();
        if (mainWindow) {
            qDebug() << "Showing MainWindow from Level1";
            mainWindow->playMusic();
            mainWindow->showLevelSelect();
        }
        return;
    }

    if (playerCar) {     //车辆控制各按键操作
        switch (event->key()) {
        case Qt::Key_O:
            playerCar->moveUp();
            event->accept();
            return;
        case Qt::Key_P:
            playerCar->moveDown();
            event->accept();
            return;
        case Qt::Key_Shift:
            playerCar->pressShift();
            if (shiftPlayer->playbackState() != QMediaPlayer::PlayingState) {
                shiftPlayer->play();
            }
            event->accept();
            return;
        default:
            break;
        }
    }

    QWidget::keyPressEvent(event);
}

void Level1::keyReleaseEvent(QKeyEvent *event)
{
    if (playerCar && event->key() == Qt::Key_Shift) {
        playerCar->releaseShift();
        shiftPlayer->stop();
    }
    QWidget::keyReleaseEvent(event);
}

bool Level1::eventFilter(QObject *obj, QEvent *event)    //事件过滤器，处理键盘事件
{
    if (obj == graphicsView->viewport() && event->type() == QEvent::KeyPress) {
        keyPressEvent(static_cast<QKeyEvent*>(event));
        return true;
    }
    if (obj == graphicsView->viewport() && event->type() == QEvent::KeyRelease) {
        keyReleaseEvent(static_cast<QKeyEvent*>(event));
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void Level1::spawnObstacle()    //生成障碍物
{
    if (isGameOver) return; 
    Obstacle::ObstacleType type = static_cast<Obstacle::ObstacleType>(QRandomGenerator::global()->bounded(0, 2));
    Obstacle::Lane lane = static_cast<Obstacle::Lane>(QRandomGenerator::global()->bounded(0, 3));
    Obstacle *obstacle = new Obstacle(type, lane, this);
    scene->addItem(obstacle);
    obstacles.append(obstacle);
}
void Level1::updateObstacles()  //更新障碍物位置
{
    if (isGameOver) return;
    qreal speedMultiplier = (playerCar && playerCar->isShifting()) ? 1.25 : 1.0;
    qreal moveSpeed = 20.0 * speedMultiplier;
    for (int i = obstacles.size() - 1; i >= 0; --i) {
        Obstacle *obstacle = obstacles[i];
        obstacle->moveLeft(moveSpeed);
    if (obstacle->isOffScreen()) {
        scene->removeItem(obstacle);
        obstacles.removeAt(i);
        delete obstacle;
        }
    }
}

void Level1::tellCollisions()  //碰撞判断
{
    if (isGameOver || !playerCar) return;
    qreal carX = playerCar->pos().x();
    qreal carWidth = playerCar->boundingRect().width();
    Obstacle::Lane carLane = static_cast<Obstacle::Lane>(static_cast<int>(playerCar->getPosition()));

    for (int i = 0; i < obstacles.size(); ++i) {
        Obstacle *obstacle = obstacles[i];
        
        if (carLane != obstacle->getLane()) {
            continue;
        }
        qreal obX = obstacle->pos().x();
        qreal obWidth = obstacle->boundingRect().width();
        bool xOverlap = (obX < carX + carWidth) && (obX + obWidth > carX);

        if (xOverlap && !obstacle->isFlashing()) {   //碰撞特效设置
            if (obstacle->getType() == Obstacle::ObstacleType::Type1) {
                if (playerCar->getState() != PlayerCar::State::Shift) {
                    obstacle->triggerFlash();
                    crashCount++;
                    crashPlayer->setPosition(0);
                    crashPlayer->play();
                    if (scoreLabel) {
                        scoreLabel->setText(QString("Crash：%1").arg(crashCount));
                    }
                }
            } else {             
                obstacle->triggerFlash();
                crashCount++;
                crashPlayer->setPosition(0);
                crashPlayer->play();
                if (scoreLabel) {
                    scoreLabel->setText(QString("Crash：%1").arg(crashCount));
                }
            }
        }
    }
}

void Level1::clearObstacles()    //清除障碍物
{
    for (Obstacle *obstacle : obstacles) {
        scene->removeItem(obstacle);
        delete obstacle;
    }
    obstacles.clear();
}

void Level1::onL1Finished()             //游戏结束判断
{
    if (l1Player->playbackState() == QMediaPlayer::StoppedState && !isGameOver) {
        isGameOver = true;
        if (gameTimer) gameTimer->stop();
        if (bgTimer) bgTimer->stop();
        if (obstacleSpawnTimer) obstacleSpawnTimer->stop();
        if (obstacleFlashTimer) obstacleFlashTimer->stop();
        windPlayer->stop();
        shiftPlayer->stop();
        showGameOverDialog();
    }
}

void Level1::showGameOverDialog()        //游戏结束小窗显示
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Game Over");
    msgBox.setText(QString("Crash：%1").arg(crashCount));
    msgBox.setStyleSheet("QLabel { color: yellow; font-size: 24px; background: transparent; } QMessageBox { background-color: lightblue; } QPushButton { background-color: lightblue; color: black; border: 1px solid gray; }");
    msgBox.setStandardButtons(QMessageBox::Ok);

    connect(&msgBox, &QMessageBox::accepted, this, &Level1::onGameOverAccepted);
    msgBox.exec();
}
void Level1::onGameOverAccepted()        
{
    clearObstacles();
    if (mainWindow) {
        mainWindow->playMusic();
        mainWindow->showLevelSelect();
    }
}

void Level1::updateGame()    //更新游戏状态
{
    if (isGameOver) return;

    gameElapsedTime += 30;

    int baseInterval = 1200;
    int minInterval = 400;
    
    double progress = static_cast<double>(gameElapsedTime) / (153 * 1000);
    if (progress > 1.0) progress = 1.0;
    int newInterval = baseInterval - static_cast<int>(progress * (baseInterval - minInterval));
    if (newInterval != obstacleSpawnInterval) {
        obstacleSpawnInterval = newInterval;
        obstacleSpawnTimer->start(obstacleSpawnInterval);
    }

    const int bpm47Interval = 1176;
    bpm47Accumulator += 30;
    if (bpm47Accumulator >= bpm47Interval) {
        bpm47Accumulator = 0;
        Obstacle *obstacle = new Obstacle(Obstacle::ObstacleType::Type1, static_cast<Obstacle::Lane>(QRandomGenerator::global()->bounded(0, 3)), this);
        scene->addItem(obstacle);
        obstacles.append(obstacle);
    }
    
    static int debugCounter = 0;
    debugCounter++;
    if (debugCounter % 100 == 0) {
        qDebug() << "GameTime:" << gameElapsedTime << "ms | Interval:" << obstacleSpawnInterval << "ms | Progress:" << progress;
    }
}

void Level1::addCrash()  //增加碰撞次数
{
    crashCount++;
    if (scoreLabel) {
        scoreLabel->setText(QString("Crash：%1").arg(crashCount));
    }
}

void Level1::scrollStreet()    //滚动街道
{
    if (isGameOver) return;
    qreal speedMultiplier = (playerCar && playerCar->isShifting()) ? 1.25 : 1.0;
    streetOffset += 20 * speedMultiplier;

    if (streetOffset >= streetWidth) {
        streetOffset = 0;
    }
    street1->setGeometry(-streetOffset, height() - streetHeight, streetWidth, streetHeight);
    street2->setGeometry(streetWidth - streetOffset, height() - streetHeight, streetWidth, streetHeight);
}

void Level1::scrollBackground()    //滚动背景，实现循环滚动
{
    if (isGameOver) return; 
    qreal speedMultiplier = (playerCar && playerCar->isShifting()) ? 1.25 : 1.0;
    bgOffset += 1 * speedMultiplier;

    if (bgOffset >= bgWidth) {
        bgOffset = 0;
    }
    
    bg1->setGeometry(-bgOffset, -220, bgWidth, bgHeight);
    bg2->setGeometry(bgWidth - bgOffset, -220, bgWidth, bgHeight);
}