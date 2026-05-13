#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include <QWidget>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui { class LevelSelect; }
QT_END_NAMESPACE

class MainWindow;

class LevelSelect : public QWidget
{
    Q_OBJECT

public:
    explicit LevelSelect(QWidget *parent = nullptr);
    ~LevelSelect();

private slots:
    void on_level1Button_clicked();
    void on_level2Button_clicked();
    void on_backButton_clicked();

private:
    Ui::LevelSelect *ui;
    MainWindow *mainWindow;
    QMediaPlayer *clickPlayer;
    QAudioOutput *clickAudioOutput;

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // LEVELSELECT_H
