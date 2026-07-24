/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionnew;
    QAction *actionopen;
    QAction *actionclose;
    QAction *actionexit;
    QAction *actionundo;
    QAction *actionres;
    QAction *actioncut;
    QAction *actioncopy;
    QAction *actionabout;
    QWidget *centralwidget;
    QGroupBox *groupBox_1;
    QLabel *label_1;
    QGroupBox *groupBox_2;
    QLabel *text_label2;
    QPushButton *takePhoto_btn;
    QPushButton *AI_reasoning_btn;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1024, 600);
        actionnew = new QAction(MainWindow);
        actionnew->setObjectName(QString::fromUtf8("actionnew"));
        actionopen = new QAction(MainWindow);
        actionopen->setObjectName(QString::fromUtf8("actionopen"));
        actionclose = new QAction(MainWindow);
        actionclose->setObjectName(QString::fromUtf8("actionclose"));
        actionexit = new QAction(MainWindow);
        actionexit->setObjectName(QString::fromUtf8("actionexit"));
        actionundo = new QAction(MainWindow);
        actionundo->setObjectName(QString::fromUtf8("actionundo"));
        actionres = new QAction(MainWindow);
        actionres->setObjectName(QString::fromUtf8("actionres"));
        actioncut = new QAction(MainWindow);
        actioncut->setObjectName(QString::fromUtf8("actioncut"));
        actioncopy = new QAction(MainWindow);
        actioncopy->setObjectName(QString::fromUtf8("actioncopy"));
        actionabout = new QAction(MainWindow);
        actionabout->setObjectName(QString::fromUtf8("actionabout"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        groupBox_1 = new QGroupBox(centralwidget);
        groupBox_1->setObjectName(QString::fromUtf8("groupBox_1"));
        groupBox_1->setGeometry(QRect(10, 0, 791, 541));
        label_1 = new QLabel(groupBox_1);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setGeometry(QRect(20, 30, 761, 501));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(809, 10, 201, 261));
        text_label2 = new QLabel(groupBox_2);
        text_label2->setObjectName(QString::fromUtf8("text_label2"));
        text_label2->setGeometry(QRect(10, 30, 181, 221));
        text_label2->setLineWidth(1);
        text_label2->setMidLineWidth(0);
        text_label2->setWordWrap(false);
        takePhoto_btn = new QPushButton(centralwidget);
        takePhoto_btn->setObjectName(QString::fromUtf8("takePhoto_btn"));
        takePhoto_btn->setGeometry(QRect(830, 300, 150, 45));
        AI_reasoning_btn = new QPushButton(centralwidget);
        AI_reasoning_btn->setObjectName(QString::fromUtf8("AI_reasoning_btn"));
        AI_reasoning_btn->setGeometry(QRect(830, 360, 150, 45));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(830, 420, 150, 45));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(830, 480, 150, 45));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionnew->setText(QApplication::translate("MainWindow", "new", nullptr));
        actionopen->setText(QApplication::translate("MainWindow", "open", nullptr));
        actionclose->setText(QApplication::translate("MainWindow", "close", nullptr));
        actionexit->setText(QApplication::translate("MainWindow", "exit", nullptr));
        actionundo->setText(QApplication::translate("MainWindow", "undo", nullptr));
        actionres->setText(QApplication::translate("MainWindow", "res", nullptr));
        actioncut->setText(QApplication::translate("MainWindow", "cut", nullptr));
        actioncopy->setText(QApplication::translate("MainWindow", "copy", nullptr));
        actionabout->setText(QApplication::translate("MainWindow", "about", nullptr));
        groupBox_1->setTitle(QApplication::translate("MainWindow", "camera1", nullptr));
        label_1->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "text", nullptr));
        text_label2->setText(QString());
        takePhoto_btn->setText(QApplication::translate("MainWindow", "takePhoto", nullptr));
        AI_reasoning_btn->setText(QApplication::translate("MainWindow", "AI reasoning", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "PushButton", nullptr));
        pushButton_4->setText(QApplication::translate("MainWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
