#ifndef LEVEL2_H
#define LEVEL2_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Level2; }
QT_END_NAMESPACE

class Level2 : public QWidget
{
    Q_OBJECT

public:
    explicit Level2(QWidget *parent = nullptr);
    ~Level2();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:
    Ui::Level2 *ui;
};

#endif // LEVEL2_H