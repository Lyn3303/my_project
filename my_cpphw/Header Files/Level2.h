#ifndef LEVEL2_H
#define LEVEL2_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QShowEvent>
#include <QEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QList>

class MainWindow;
class PlayerCar;
class Obstacle;

class Level2 : public QWidget
{
    Q_OBJECT

public:
    explicit Level2(QWidget *parent = nullptr);
    ~Level2();
    void addCrash();

public slots:
    void startLevelMusic();
    void stopLevelMusic();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void updateGame();
    void scrollStreet();
    void scrollBackground();
    void onL2Finished();
    void onShiftFinished();
    void onGameOverAccepted();
    void spawnObstacle();
    void updateObstacles();

private:
    void showGameOverDialog();
    void checkCollisions();
    void clearObstacles();

    QLabel *street1;
    QLabel *street2;
    QLabel *scoreLabel;
    QLabel *bg1;
    QLabel *bg2;
    QTimer *gameTimer;
    QTimer *bgTimer;
    QTimer *obstacleSpawnTimer;
    MainWindow *mainWindow;
    QPixmap scaledStreetPixmap;
    int streetOffset;
    int bgOffset;
    int streetWidth;
    int streetHeight;
    int bgWidth;
    int bgHeight;
    int crashCount;
    QGraphicsScene *scene;
    QGraphicsView *graphicsView;
    PlayerCar *playerCar;
    QList<Obstacle*> obstacles;

    QMediaPlayer *l2Player;
    QAudioOutput *l2AudioOutput;
    QMediaPlayer *windPlayer;
    QAudioOutput *windAudioOutput;
    QMediaPlayer *shiftPlayer;
    QAudioOutput *shiftAudioOutput;
    bool isGameOver;
    int obstacleSpawnInterval;
    int lastObstacleType;
    int windPlayTime;
    int bpmAccumulator;
    int bpm47Accumulator;
    QTimer *obstacleFlashTimer;
    qint64 gameElapsedTime;
};

#endif // LEVEL2_H