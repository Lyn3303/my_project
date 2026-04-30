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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void stopMusic() { player->stop(); }
    void playMusic() { player->play(); }

private slots:
    void on_startButton_clicked();
    void on_operateButton_clicked();

private:
    Ui::MainWindow *ui;
    LevelSelect *levelSelect;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QWidget *operateWidget;

    void showLevelSelect();
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
