#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include <QWidget>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class LevelSelect; }
QT_END_NAMESPACE

class Level1;
class Level2;

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
    Level1 *level1;
    Level2 *level2;

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // LEVELSELECT_H