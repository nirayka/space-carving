/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *selectImagesButton;
    QPushButton *selectMetadataButton;
    QPushButton *parseButton;
    QPushButton *startButton;
    QMenuBar *menubar;
    QMenu *menuSpace_Carver;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        QFont font;
        font.setFamilies({QString::fromUtf8("Consolas")});
        font.setPointSize(12);
        font.setKerning(true);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        selectImagesButton = new QPushButton(centralwidget);
        selectImagesButton->setObjectName("selectImagesButton");
        selectImagesButton->setGeometry(QRect(210, 80, 171, 41));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Consolas")});
        font1.setPointSize(14);
        font1.setKerning(true);
        selectImagesButton->setFont(font1);
        selectMetadataButton = new QPushButton(centralwidget);
        selectMetadataButton->setObjectName("selectMetadataButton");
        selectMetadataButton->setGeometry(QRect(390, 80, 171, 41));
        selectMetadataButton->setFont(font1);
        parseButton = new QPushButton(centralwidget);
        parseButton->setObjectName("parseButton");
        parseButton->setGeometry(QRect(210, 130, 171, 41));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Consolas")});
        font2.setPointSize(16);
        font2.setItalic(true);
        font2.setKerning(true);
        parseButton->setFont(font2);
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(390, 130, 171, 41));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Consolas")});
        font3.setPointSize(14);
        font3.setBold(true);
        font3.setKerning(true);
        startButton->setFont(font3);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menuSpace_Carver = new QMenu(menubar);
        menuSpace_Carver->setObjectName("menuSpace_Carver");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSpace_Carver->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        selectImagesButton->setText(QCoreApplication::translate("MainWindow", "select images", nullptr));
        selectMetadataButton->setText(QCoreApplication::translate("MainWindow", "select metadata", nullptr));
        parseButton->setText(QCoreApplication::translate("MainWindow", "parse", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "start carving!", nullptr));
        menuSpace_Carver->setTitle(QCoreApplication::translate("MainWindow", "\342\230\205 Nirayka's Space Carver \342\230\205", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
