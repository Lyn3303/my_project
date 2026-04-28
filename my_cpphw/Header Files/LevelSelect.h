#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LevelSelect; }
QT_END_NAMESPACE

class LevelSelect : public QWidget
{
    Q_OBJECT

public:
    explicit LevelSelect(QWidget *parent = nullptr);
    ~LevelSelect();

signals:
    void levelSelected(int level);

private slots:
    void on_level1Button_clicked();
    void on_level2Button_clicked();

private:
    Ui::LevelSelect *ui;
};

#endif // LEVELSELECT_H