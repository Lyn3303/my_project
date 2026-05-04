#include "Level2.h"
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

Level2::Level2(QWidget *parent)
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
    setStyleSheet("background-color: black;");

    l2Player = new QMediaPlayer(this);
    l2AudioOutput = new QAudioOutput(this);
    l2Player->setAudioOutput(l2AudioOutput);
    l2AudioOutput->setVolume(0.8);
    connect(l2Player, &QMediaPlayer::playbackStateChanged, this, &Level2::onL2Finished);

    windPlayer = new QMediaPlayer(this);
    windAudioOutput = new QAudioOutput(this);
    windPlayer->setAudioOutput(windAudioOutput);
    windAudioOutput->setVolume(0.1);
    windPlayer->setLoops(QMediaPlayer::Infinite);

    shiftPlayer = new QMediaPlayer(this);
    shiftAudioOutput = new QAudioOutput(this);
    shiftPlayer->setAudioOutput(shiftAudioOutput);
    shiftAudioOutput->setVolume(0.3);
    shiftPlayer->setLoops(QMediaPlayer::Infinite);
    connect(shiftPlayer, &QMediaPlayer::playbackStateChanged, this, &Level2::onShiftFinished);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1280, 720);
    scene->setBackgroundBrush(Qt::NoBrush);

    QGraphicsView *view = new QGraphicsView(scene, this);
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

    scoreLabel = new QLabel(this);
    scoreLabel->setGeometry(20, 20, 200, 30);
    scoreLabel->setStyleSheet("font-size: 20px; color: rgb(255, 248, 32); background: transparent;");
    scoreLabel->setText("Crash：0");

    qDebug() << "Level2 constructed";
}

void Level2::startLevelMusic()
{
    l2Player->setSource(QUrl("qrc:/music/L2.mp3"));
    windPlayer->setSource(QUrl("qrc:/sound/wind.mp3"));
    shiftPlayer->setSource(QUrl("qrc:/sound/shift.mp3"));

    l2Player->play();
    windPlayer->play();

    obstacleSpawnTimer = new QTimer(this);
    connect(obstacleSpawnTimer, &QTimer::timeout, this, &Level2::spawnObstacle);
    obstacleSpawnTimer->start(obstacleSpawnInterval);

    obstacleFlashTimer = new QTimer(this);
    obstacleFlashTimer->setInterval(50);
    connect(obstacleFlashTimer, &QTimer::timeout, this, [this]() {
        for (Obstacle *ob : obstacles) {
            if (ob->isFlashing()) {
                ob->updateFlash();
            }
        }
    });
    obstacleFlashTimer->start();
}

void Level2::stopLevelMusic()
{
    l2Player->stop();
    windPlayer->stop();
    shiftPlayer->stop();
    if (obstacleSpawnTimer) {
        obstacleSpawnTimer->stop();
    }
    if (obstacleFlashTimer) {
        obstacleFlashTimer->stop();
    }
}

void Level2::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    qDebug() << "Level2 shown, size:" << size();

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
        connect(gameTimer, &QTimer::timeout, this, &Level2::scrollStreet);
        connect(gameTimer, &QTimer::timeout, this, &Level2::updateGame);
        connect(gameTimer, &QTimer::timeout, this, &Level2::updateObstacles);
        connect(gameTimer, &QTimer::timeout, this, &Level2::checkCollisions);
        gameTimer->start(30);
    }

    if (bg1 == nullptr && bg2 == nullptr) {
        QPixmap bgPixmap(":/bk/L3.png");
        qDebug() << "BG L3 loaded:" << !bgPixmap.isNull();
        qDebug() << "BG original size:" << bgPixmap.size();

        bgWidth = bgPixmap.width() * 0.8;
        bgHeight = bgPixmap.height() * 0.8 * 2 / 3;
        QPixmap scaledBg = bgPixmap.scaledToHeight(bgHeight);  // 只按高度缩放，保持比例
        bgWidth = scaledBg.width();  // 更新宽度为实际缩放后的宽度
        bgHeight = scaledBg.height();
        
        qDebug() << "BG scaled size:" << scaledBg.size();

        bg1 = new QLabel(this);
        bg1->setPixmap(scaledBg);
        bg1->setGeometry(0, 0, bgWidth, bgHeight);
        bg1->show();
        bg1->lower();

        bg2 = new QLabel(this);
        bg2->setPixmap(scaledBg);
        bg2->setGeometry(bgWidth, 0, bgWidth, bgHeight);
        bg2->show();
        bg2->lower();

        bgTimer = new QTimer(this);
        connect(bgTimer, &QTimer::timeout, this, &Level2::scrollBackground);
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

Level2::~Level2()
{
    stopLevelMusic();
    clearObstacles();
}

void Level2::keyPressEvent(QKeyEvent *event)
{
    if (isGameOver) {
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Escape pressed in Level2";
        if (gameTimer) gameTimer->stop();
        if (bgTimer) bgTimer->stop();
        stopLevelMusic();
        clearObstacles();
        if (mainWindow) {
            qDebug() << "Showing MainWindow from Level2";
            mainWindow->playMusic();
            mainWindow->showLevelSelect();
        }
        return;
    }

    if (playerCar) {
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

void Level2::keyReleaseEvent(QKeyEvent *event)
{
    if (playerCar && event->key() == Qt::Key_Shift) {
        playerCar->releaseShift();
        shiftPlayer->stop();
    }
    QWidget::keyReleaseEvent(event);
}

bool Level2::eventFilter(QObject *obj, QEvent *event)
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

void Level2::spawnObstacle()
{
    if (isGameOver) return;

    Obstacle::ObstacleType type = static_cast<Obstacle::ObstacleType>(QRandomGenerator::global()->bounded(0, 2));

    Obstacle::Lane lane = static_cast<Obstacle::Lane>(QRandomGenerator::global()->bounded(0, 3));

    Obstacle *obstacle = new Obstacle(type, lane, this);
    scene->addItem(obstacle);
    obstacles.append(obstacle);
}

void Level2::updateObstacles()
{
    if (isGameOver) return;

    qreal moveSpeed = 20.0;

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

void Level2::checkCollisions()
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

        if (xOverlap && !obstacle->isFlashing()) {
            if (obstacle->getType() == Obstacle::ObstacleType::Type1) {
                if (playerCar->getState() != PlayerCar::State::Shift) {
                    obstacle->triggerFlash();
                    crashCount++;
                    if (scoreLabel) {
                        scoreLabel->setText(QString("Crash：%1").arg(crashCount));
                    }
                }
            } else {
                obstacle->triggerFlash();
                crashCount++;
                if (scoreLabel) {
                    scoreLabel->setText(QString("Crash：%1").arg(crashCount));
                }
            }
        }
    }
}

void Level2::clearObstacles()
{
    for (Obstacle *obstacle : obstacles) {
        scene->removeItem(obstacle);
        delete obstacle;
    }
    obstacles.clear();
}

void Level2::onL2Finished()
{
    if (l2Player->playbackState() == QMediaPlayer::StoppedState && !isGameOver) {
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

void Level2::onShiftFinished()
{
}

void Level2::showGameOverDialog()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Game Over");
    msgBox.setText(QString("Crash：%1").arg(crashCount));
    msgBox.setStyleSheet("QLabel { color: yellow; font-size: 24px; background: transparent; } QMessageBox { background-color: lightblue; } QPushButton { background-color: lightblue; color: black; border: 1px solid gray; }");
    msgBox.setStandardButtons(QMessageBox::Ok);

    connect(&msgBox, &QMessageBox::accepted, this, &Level2::onGameOverAccepted);
    msgBox.exec();
}

void Level2::onGameOverAccepted()
{
    clearObstacles();
    if (mainWindow) {
        mainWindow->playMusic();
        mainWindow->showLevelSelect();
    }
}

void Level2::updateGame()
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
        qDebug() << "Level2 GameTime:" << gameElapsedTime << "ms | Interval:" << obstacleSpawnInterval << "ms | Progress:" << progress;
    }
}

void Level2::addCrash()
{
    crashCount++;
    if (scoreLabel) {
        scoreLabel->setText(QString("Crash：%1").arg(crashCount));
    }
}

void Level2::scrollStreet()
{
    if (isGameOver) return;

    streetOffset += 20;

    if (streetOffset >= streetWidth) {
        streetOffset = 0;
    }

    street1->setGeometry(-streetOffset, height() - streetHeight, streetWidth, streetHeight);
    street2->setGeometry(streetWidth - streetOffset, height() - streetHeight, streetWidth, streetHeight);
}

void Level2::scrollBackground()
{
    if (isGameOver) return;

    bgOffset += 1;

    if (bgOffset >= bgWidth) {
        bgOffset = 0;
        qDebug() << "BG reset, bgWidth:" << bgWidth;
    }

    bg1->setGeometry(-bgOffset, 0, bgWidth, bgHeight);
    bg2->setGeometry(bgWidth - bgOffset, 0, bgWidth, bgHeight);
}
