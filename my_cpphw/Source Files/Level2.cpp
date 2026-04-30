#include "Level2.h"
#include "ui_Level2.h"
#include "mainwindow.h"
#include <QKeyEvent>

Level2::Level2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Level2)
{
    ui->setupUi(this);
}

Level2::~Level2()
{
    delete ui;
}

void Level2::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->hide();
        if (parentWidget()) {
            parentWidget()->show();
            MainWindow *mainWindow = qobject_cast<MainWindow*>(parentWidget()->parentWidget());
            if (mainWindow) {
                mainWindow->playMusic();
            }
        }
    }
    QWidget::keyPressEvent(event);
}

void Level2::updateGame()
{
}