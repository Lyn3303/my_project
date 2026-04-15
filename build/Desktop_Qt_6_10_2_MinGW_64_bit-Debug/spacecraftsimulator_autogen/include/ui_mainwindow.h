/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *backgroundLabel;
    QPushButton *startButton;
    QLabel *title;
    QFrame *line;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        backgroundLabel = new QLabel(centralwidget);
        backgroundLabel->setObjectName("backgroundLabel");
        backgroundLabel->setGeometry(QRect(0, 0, 800, 600));
        backgroundLabel->setStyleSheet(QString::fromUtf8("background-image: url(:/images/mainback.jpg);\n"
"background-repeat: no-repeat;\n"
"background-position: center;\n"
"background-size: cover;"));
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(160, 320, 451, 121));
        startButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    border: none;\n"
"    color: white;\n"
"    font-size: 24px;\n"
"    font-weight: bold;\n"
"    text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.5);\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    color: #00ffff;\n"
"    text-shadow: 0 0 10px #00ffff, 0 0 20px #00ffff;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    color: #0077ffff;\n"
"    text-shadow: 0 0 15px #4400ffff, 0 0 30px #2600ffff;\n"
"}"));
        title = new QLabel(centralwidget);
        title->setObjectName("title");
        title->setGeometry(QRect(80, 60, 651, 151));
        title->setMinimumSize(QSize(101, 0));
        QFont font;
        font.setFamilies({QString::fromUtf8("ISOCP")});
        font.setPointSize(89);
        font.setBold(false);
        font.setItalic(true);
        title->setFont(font);
        title->setStyleSheet(QString::fromUtf8("\n"
"font: italic 89pt \"ISOCP\" !important;\n"
"color: rgba(255, 255, 255, 0.9); /* \344\270\273\346\226\207\345\255\227\347\231\275\350\211\262\357\274\214\345\270\246\351\200\217\346\230\216\345\272\246 */\n"
"color: qconicalgradient(cx:0.454545, cy:0.54, angle:329.8, stop:0 rgba(206, 206, 206, 255), stop:0.16 rgba(76, 1, 138, 255), stop:0.225 rgba(120, 23, 158, 255), stop:0.285 rgba(147, 41, 189, 255), stop:0.345 rgba(146, 84, 235, 255), stop:0.415 rgba(126, 117, 245, 255), stop:0.52 rgba(68, 12, 209, 255), stop:0.57 rgba(5, 41, 106, 255), stop:0.635 rgba(42, 44, 168, 255), stop:0.695 rgba(31, 38, 155, 255), stop:0.744318 rgba(46, 86, 191, 255), stop:0.815 rgba(37, 83, 239, 255), stop:0.88 rgba(64, 131, 233, 255), stop:0.880682 rgba(29, 174, 250, 255), stop:1 rgba(21, 129, 209, 255));"));
        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setGeometry(QRect(20, 400, 731, 20));
        line->setStyleSheet(QString::fromUtf8("\n"
"background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(36, 208, 255, 206), stop:0.289773 rgba(58, 120, 255, 69), stop:0.505682 rgba(44, 237, 255, 156), stop:0.528409 rgba(66, 68, 255, 199), stop:1 rgba(255, 255, 255, 0));"));
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Raised);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        backgroundLabel->setText(QString());
        startButton->setText(QCoreApplication::translate("MainWindow", "click to start", nullptr));
        title->setText(QCoreApplication::translate("MainWindow", "Abyss 2047", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
