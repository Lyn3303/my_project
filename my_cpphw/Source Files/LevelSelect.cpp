#include "LevelSelect.h"
#include "ui_LevelSelect.h"
#include "mainwindow.h"
#include <QPixmap>

LevelSelect::LevelSelect(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelSelect)
    , mainWindow(qobject_cast<MainWindow*>(parent))
{
    ui->setupUi(this);
    setFixedSize(1280, 720);
    ui->line->setPixmap(QPixmap(":/ui/divider-003.png"));
    ui->line->setScaledContents(true);
    ui->backgroundLabel->setPixmap(QPixmap(":/bk/backgound.png"));
    ui->backgroundLabel->setScaledContents(true);
    ui->backgroundLabel->lower();
    
    clickPlayer = new QMediaPlayer(this);
    clickAudioOutput = new QAudioOutput(this);
    clickPlayer->setAudioOutput(clickAudioOutput);
    clickPlayer->setSource(QUrl("qrc:/sound/click.mp3"));
    clickAudioOutput->setVolume(1.0);
}

LevelSelect::~LevelSelect()
{
    delete ui;
}

void LevelSelect::keyPressEvent(QKeyEvent *event)   //按键响应
{
    if (clickPlayer) {
        clickPlayer->stop();
        clickPlayer->play();
    }
    if (event->key() == Qt::Key_Escape) {
        on_backButton_clicked();
    }
    QWidget::keyPressEvent(event);
}

void LevelSelect::on_level1Button_clicked()  //关卡一跳转
{
    if (clickPlayer) {      
        clickPlayer->stop();
        clickPlayer->play();
    }
    if (mainWindow) {
        mainWindow->stopMusic();
        mainWindow->showLevel1();
    }
}

void LevelSelect::on_level2Button_clicked()  //关卡二跳转
{
    if (clickPlayer) {
        clickPlayer->stop();
        clickPlayer->play();
    }
    if (mainWindow) {
        mainWindow->stopMusic();
        mainWindow->showLevel2();
    }
}

void LevelSelect::on_backButton_clicked()  //返回主菜单
{
    if (clickPlayer) {      
        clickPlayer->stop();
        clickPlayer->play();
    }
    if (mainWindow) {
        mainWindow->showMainWindow();
    }
}
