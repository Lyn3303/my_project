#include "Level1.h"
#include "ui_Level1.h"
#include "mainwindow.h"
#include <QKeyEvent>

Level1::Level1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Level1)
{
    ui->setupUi(this);
}

Level1::~Level1()
{
    delete ui;
}

void Level1::keyPressEvent(QKeyEvent *event)
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
        return;
    }
    QWidget::keyPressEvent(event);
}

void Level1::updateGame()
{
}