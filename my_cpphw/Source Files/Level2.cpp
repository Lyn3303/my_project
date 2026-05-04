#include "Level2.h"
#include "mainwindow.h"
#include "LevelSelect.h"
#include <QKeyEvent>
#include <QPixmap>
#include <QShowEvent>
#include <QDebug>
#include <QLabel>

Level2::Level2(QWidget *parent)
    : QWidget(parent)
    , streetOffset(0)
    , crashCount(0)
    , street1(nullptr)
    , street2(nullptr)
    , gameTimer(nullptr)
    , scoreLabel(nullptr)
    , mainWindow(qobject_cast<MainWindow*>(parent))
{
    setFixedSize(1280, 720);
    setStyleSheet("background-color: black;");

    scoreLabel = new QLabel(this);
    scoreLabel->setGeometry(20, 20, 200, 30);
    scoreLabel->setStyleSheet("font-size: 20px; color: rgb(255, 248, 32);");
    scoreLabel->setText("Crash：0");

    qDebug() << "Level2 constructed";
}

void Level2::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    qDebug() << "Level2 shown, size:" << size();

    if (street1 == nullptr && street2 == nullptr) {
        QPixmap streetPixmap(":/content/street.PNG");
        qDebug() << "Pixmap loaded:" << !streetPixmap.isNull();
        qDebug() << "Pixmap size:" << streetPixmap.size();
        if (!streetPixmap.isNull()) {
            scaledStreetPixmap = streetPixmap.scaledToWidth(1280);
            streetHeight = scaledStreetPixmap.height();
            streetWidth = scaledStreetPixmap.width();

            street1 = new QLabel(this);
            street1->setPixmap(scaledStreetPixmap);
            street1->setGeometry(0, height() - streetHeight, streetWidth, streetHeight);
            street1->show();

            street2 = new QLabel(this);
            street2->setPixmap(scaledStreetPixmap);
            street2->setGeometry(streetWidth, height() - streetHeight, streetWidth, streetHeight);
            street2->show();
        }

        gameTimer = new QTimer(this);
        connect(gameTimer, &QTimer::timeout, this, &Level2::scrollStreet);
    }
    if (gameTimer) {
        gameTimer->start(30);
    }
}

Level2::~Level2()
{
    delete street1;
    delete street2;
    delete gameTimer;
    delete scoreLabel;
}

void Level2::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Escape pressed in Level2";
        gameTimer->stop();
        if (mainWindow) {
            qDebug() << "Showing MainWindow from Level2";
            mainWindow->playMusic();
            mainWindow->showLevelSelect();
        }
        return;
    }
    QWidget::keyPressEvent(event);
}

void Level2::updateGame()
{
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
    streetOffset += 10;

    if (streetOffset >= streetWidth) {
        streetOffset = 0;
    }

    street1->setGeometry(-streetOffset, height() - streetHeight, streetWidth, streetHeight);
    street2->setGeometry(streetWidth - streetOffset, height() - streetHeight, streetWidth, streetHeight);
}
