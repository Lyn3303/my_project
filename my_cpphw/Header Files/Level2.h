#ifndef LEVEL2_H
#define LEVEL2_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QShowEvent>

class MainWindow;

class Level2 : public QWidget
{
    Q_OBJECT

public:
    explicit Level2(QWidget *parent = nullptr);
    ~Level2();
    void addCrash();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void updateGame();
    void scrollStreet();

private:
    QLabel *street1;
    QLabel *street2;
    QLabel *scoreLabel;
    QTimer *gameTimer;
    MainWindow *mainWindow;
    QPixmap scaledStreetPixmap;
    int streetOffset;
    int streetWidth;
    int streetHeight;
    int crashCount;
};

#endif // LEVEL2_H
