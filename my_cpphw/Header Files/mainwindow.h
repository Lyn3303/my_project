#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class LevelSelect;
class Level1;
class Level2;
class Level3;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_levelSelected(int level);
    void on_backToMenu();

private:
    Ui::MainWindow *ui;
    LevelSelect *levelSelect;
    Level1 *level1;
    Level2 *level2;
    Level3 *level3;

    void showMainMenu();
    void showLevelSelect();
    void showLevel(int level);
};
#endif // MAINWINDOW_H
