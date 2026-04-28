#include "LevelSelect.h"
#include "ui_LevelSelect.h"

LevelSelect::LevelSelect(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelSelect)
{
    ui->setupUi(this);
}

LevelSelect::~LevelSelect()
{
    delete ui;
}

void LevelSelect::on_level1Button_clicked()
{
    emit levelSelected(1);
}

void LevelSelect::on_level2Button_clicked()
{
    emit levelSelected(2);
}

void LevelSelect::on_level3Button_clicked()
{
    emit levelSelected(3);
}