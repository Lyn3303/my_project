#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LevelSelect.h"
#include "Level1.h"
#include "Level2.h"
#include <QApplication>
#include <QKeyEvent>
#include <QScreen>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QUrl>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    //设置UI
    setFixedSize(1280, 720);
    setWindowTitle("Night Way");
    setWindowIcon(QIcon(":/bk/icon.ico"));

    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        move((screen->geometry().width() - width()) / 2, (screen->geometry().height() - height()) / 2);
    }

    levelSelect = new LevelSelect(this);
    levelSelect->setFixedSize(1280, 720);
    levelSelect->hide();

    level1 = new Level1(this);
    level1->setFixedSize(1280, 720);
    level1->hide();

    level2 = new Level2(this);
    level2->setFixedSize(1280, 720);
    level2->hide();

    operateWidget = new QWidget(this);
    operateWidget->setGeometry(300, 200, 680, 320);
    operateWidget->setStyleSheet("background-color: rgba(99, 98, 98, 0.8); border: 2px solid white;");
    operateWidget->hide();

    QVBoxLayout *layout = new QVBoxLayout(operateWidget);
    QLabel *titleLabel = new QLabel("Operation：\n Key:O 上移动 \n Key:P 下移动 \n shift 加速,可用于通过蓝色弹幕 \n esc 退出游戏 \n 再次点击operation 关闭此窗口 ", operateWidget);
    titleLabel->setStyleSheet("color: yellow; font: 29pt \"Book Antiqua\";");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    player = new QMediaPlayer(this);    //创建音乐播放器
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    QUrl musicUrl("qrc:/music/mainwindow.mp3");
    qDebug() << "Music URL:" << musicUrl;
    qDebug() << "Music URL valid:" << musicUrl.isValid();
    player->setSource(musicUrl);
    player->setLoops(QMediaPlayer::Infinite);
    audioOutput->setVolume(0.3);
    qDebug() << "Starting music playback...";
    player->play();

    clickPlayer = new QMediaPlayer(this);    //点击音效播放器
    clickAudioOutput = new QAudioOutput(this);
    clickPlayer->setAudioOutput(clickAudioOutput);
    clickPlayer->setSource(QUrl("qrc:/sound/click.mp3"));
    clickAudioOutput->setVolume(1.0);
}

void MainWindow::playMusic()    //播放音乐
{
    qDebug() << "Playing music, current state:" << player->playbackState();
    if (player->playbackState() == QMediaPlayer::StoppedState) {
        player->play();
    } else if (player->playbackState() == QMediaPlayer::PausedState) {
        player->play();
    }
}

void MainWindow::stopMusic()   //停止播放
{
    qDebug() << "Stopping music";
    player->stop();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete levelSelect;
    delete level1;
    delete level2;
}

void MainWindow::on_startButton_clicked()    //开始游戏按键响应
{
    if (clickPlayer) {
        clickPlayer->stop();
        clickPlayer->play();
    }
    showLevelSelect();
}

void MainWindow::on_operateButton_clicked()    //操作教程按键响应
{
    if (clickPlayer) {
        clickPlayer->stop();
        clickPlayer->play();
    }
    if (operateWidget->isVisible()) {
        operateWidget->hide();
    } else {
        operateWidget->show();
    }
}

void MainWindow::showLevelSelect()  //跳转到选择关卡界面
{
    ui->centralwidget->hide();
    level1->hide();
    level2->hide();
    levelSelect->show();
    levelSelect->raise();
}

void MainWindow::showLevel1()    //跳转到关卡1界面
{
    ui->centralwidget->hide();
    levelSelect->hide();
    level2->hide();
    level1->show();
    level1->raise();
    level1->startLevelMusic();
}

void MainWindow::showLevel2()  //跳转到关卡2界面
{
    ui->centralwidget->hide();
    levelSelect->hide();
    level1->hide();
    level2->show();
    level2->raise();
    level2->startLevelMusic();
}

void MainWindow::showMainWindow()    //跳转回主界面
{
    levelSelect->hide();
    level1->hide();
    level2->hide();
    ui->centralwidget->show();
    ui->centralwidget->raise();
}

void MainWindow::keyPressEvent(QKeyEvent *event)   
{
    if (clickPlayer) {
        clickPlayer->stop();
        clickPlayer->play();
    }
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {  //按键响应
        if (ui->centralwidget->isVisible()) {
            showLevelSelect();
        }
    }
    QMainWindow::keyPressEvent(event);
}
