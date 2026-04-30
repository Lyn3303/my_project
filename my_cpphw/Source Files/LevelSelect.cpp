#include "LevelSelect.h"
#include "ui_LevelSelect.h"
#include "Level1.h"
#include "Level2.h"
#include "mainwindow.h"
#include <QPixmap>

LevelSelect::LevelSelect(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelSelect)
{
    ui->setupUi(this);
    setGeometry(0, 0, parentWidget()->width(), parentWidget()->height());
    level1 = new Level1(this);
    level2 = new Level2(this);
    level1->setGeometry(0, 0, width(), height());
    level2->setGeometry(0, 0, width(), height());
    level1->hide();
    level2->hide();

    ui->line->setPixmap(QPixmap(":/ui/divider-003.png"));
    ui->line->setScaledContents(true);

    ui->backgroundLabel->setPixmap(QPixmap(":/bk/backgound.png"));
    ui->backgroundLabel->setScaledContents(true);
    ui->backgroundLabel->lower();
}

LevelSelect::~LevelSelect()
{
    delete ui;
    delete level1;
    delete level2;
}

void LevelSelect::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        on_backButton_clicked();
    }
    QWidget::keyPressEvent(event);
}

void LevelSelect::on_level1Button_clicked()
{
    MainWindow *mainWindow = qobject_cast<MainWindow*>(parentWidget());
    if (mainWindow) {
        mainWindow->stopMusic();
    }
    this->hide();
    level1->setGeometry(0, 0, parentWidget()->width(), parentWidget()->height());
    level1->show();
}

void LevelSelect::on_level2Button_clicked()
{
    MainWindow *mainWindow = qobject_cast<MainWindow*>(parentWidget());
    if (mainWindow) {
        mainWindow->stopMusic();
    }
    this->hide();
    level2->setGeometry(0, 0, parentWidget()->width(), parentWidget()->height());
    level2->show();
}

void LevelSelect::on_backButton_clicked()
{
    level1->hide();
    level2->hide();
    this->hide();
    if (parentWidget()) {
        MainWindow *mainWindow = qobject_cast<MainWindow*>(parentWidget());
        if (mainWindow) {
            mainWindow->playMusic();
        }
        parentWidget()->show();
    }
}