#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QWidget>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class LevelSelect;
class Level1;
class Level2;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void stopMusic();
    void playMusic();
    void showLevelSelect();
    void showLevel1();
    void showLevel2();
    void showMainWindow();

private slots:
    void on_startButton_clicked();
    void on_operateButton_clicked();

private:
    Ui::MainWindow *ui;
    LevelSelect *levelSelect;
    Level1 *level1;
    Level2 *level2;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QMediaPlayer *clickPlayer;
    QAudioOutput *clickAudioOutput;
    QWidget *operateWidget;

    void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
