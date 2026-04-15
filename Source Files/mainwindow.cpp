#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GameWidget.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // 设置主窗口背景
    QPixmap backgroundPixmap(":/images/background.png");
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(backgroundPixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)));
    this->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    // 创建游戏主界面的实例
    GameWidget *gameWidget = new GameWidget;
    // 显示游戏主界面
    gameWidget->show();
    // 隐藏当前的主界面（开始界面）
    this->hide();
}
