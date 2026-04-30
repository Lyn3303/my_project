#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LevelSelect.h"
#include <QApplication>
#include <QKeyEvent>
#include <QScreen>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1280, 720);  
    setWindowTitle("my_cpphw");

    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        move((screen->geometry().width() - width()) / 2, (screen->geometry().height() - height()) / 2);
    }

    levelSelect = new LevelSelect(this);
    levelSelect->hide();

    operateWidget = new QWidget(this);
    operateWidget->setGeometry(300, 200, 680, 320);
    operateWidget->setStyleSheet("background-color: rgba(99, 98, 98, 0.8); border: 2px solid white;");
    operateWidget->hide();

    QVBoxLayout *layout = new QVBoxLayout(operateWidget);
    QLabel *titleLabel = new QLabel("Operation：\n PgUp 上移动 \n PgDown 下移动 \n shift 加速,可用于通过粉色弹幕 \n esc 退出游戏 \n 再次点击operation 关闭此窗口 " , operateWidget);
    titleLabel->setStyleSheet("color: yellow; font: 29pt \"Book Antiqua\";");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl("qrc:/music/mainwindow.mp3"));
    player->setLoops(QMediaPlayer::Infinite);
    audioOutput->setVolume(0.3);
    player->play();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete levelSelect;
}

void MainWindow::on_startButton_clicked()
{
    showLevelSelect();
}

void MainWindow::on_operateButton_clicked()
{
    if (operateWidget->isVisible()) {  //操作逻辑窗口可见时，点击操作按钮隐藏窗口
        operateWidget->hide();
    } else {
        operateWidget->show();
    }
}

void MainWindow::showLevelSelect()
{
    ui->centralwidget->hide();  //隐藏主窗口
    levelSelect->setGeometry(0, 0, width(), height());
    levelSelect->show();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        if (ui->centralwidget->isVisible()) {
            showLevelSelect();
        }
    }
    QMainWindow::keyPressEvent(event);
}