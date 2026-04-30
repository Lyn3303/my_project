#ifndef LEVEL1_H
#define LEVEL1_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Level1; }
QT_END_NAMESPACE

class Level1 : public QWidget
{
    Q_OBJECT

public:
    explicit Level1(QWidget *parent = nullptr);
    ~Level1();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:
    Ui::Level1 *ui;
};

#endif // LEVEL1_H