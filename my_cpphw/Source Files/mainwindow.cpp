#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1280, 720);
    setWindowTitle("my_cpphw");
    move((QApplication::desktop()->width() - width()) / 2, (QApplication::desktop()->height() - height()) / 2);
}

MainWindow::~MainWindow()
{
    delete ui;
}
