#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LevelSelect.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1280, 720);
    setWindowTitle("my_cpphw");
    move((QApplication::desktop()->width() - width()) / 2, (QApplication::desktop()->height() - height()) / 2);

    levelSelect = new LevelSelect(this);
    level1 = new Level1(this);
    level2 = new Level2(this);
    level3 = new Level3(this);

    connect(levelSelect, &LevelSelect::levelSelected, this, &MainWindow::on_levelSelected);
    connect(level1, &Level1::backToMenu, this, &MainWindow::on_backToMenu);
    connect(level2, &Level2::backToMenu, this, &MainWindow::on_backToMenu);
    connect(level3, &Level3::backToMenu, this, &MainWindow::on_backToMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete levelSelect;
    delete level1;
    delete level2;
    delete level3;
}

void MainWindow::on_startButton_clicked()
{
    showLevelSelect();
}

void MainWindow::on_levelSelected(int level)
{
    showLevel(level);
}

void MainWindow::on_backToMenu()
{
    showMainMenu();
}

void MainWindow::showMainMenu()
{
    levelSelect->hide();
    level1->hide();
    level2->hide();
    level3->hide();
    ui->centralwidget->show();
}

void MainWindow::showLevelSelect()
{
    ui->centralwidget->hide();
    level1->hide();
    level2->hide();
    level3->hide();
    levelSelect->show();
    levelSelect->setGeometry(0, 0, width(), height());
}

void MainWindow::showLevel(int level)
{
    ui->centralwidget->hide();
    levelSelect->hide();
    level1->hide();
    level2->hide();
    level3->hide();

    switch (level) {
    case 1:
        level1->show();
        level1->setGeometry(0, 0, width(), height());
        break;
    case 2:
        level2->show();
        level2->setGeometry(0, 0, width(), height());
        break;
    case 3:
        level3->show();
        level3->setGeometry(0, 0, width(), height());
        break;
    }
}
