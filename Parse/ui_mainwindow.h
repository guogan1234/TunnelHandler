/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QWidget *toolW1;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton_6;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_18;
    QPushButton *pushButton_19;
    QPushButton *pushButton_16;
    QPushButton *pushButton_5;
    QPushButton *pushButton_17;
    QSpacerItem *horizontalSpacer;
    QPushButton *change;
    QWidget *toolW2;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_9;
    QLineEdit *Index;
    QPushButton *pushButton_7;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QPushButton *pushButton_8;
    QPushButton *pushButton_20;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QWidget *dataW;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *XMin;
    QLineEdit *XMax;
    QPushButton *pushButton_10;
    QLabel *label_2;
    QLineEdit *YMin;
    QLineEdit *YMax;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *handleCJL_btn;
    QPushButton *pushButton_21;
    QPushButton *pushButton_11;
    QCustomPlot *plot;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1000, 800);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        toolW1 = new QWidget(centralWidget);
        toolW1->setObjectName(QStringLiteral("toolW1"));
        toolW1->setStyleSheet(QStringLiteral(""));
        verticalLayout = new QVBoxLayout(toolW1);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(toolW1);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_6 = new QPushButton(groupBox);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        horizontalLayout->addWidget(pushButton_6);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(groupBox);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout->addWidget(pushButton_4);

        pushButton_18 = new QPushButton(groupBox);
        pushButton_18->setObjectName(QStringLiteral("pushButton_18"));

        horizontalLayout->addWidget(pushButton_18);

        pushButton_19 = new QPushButton(groupBox);
        pushButton_19->setObjectName(QStringLiteral("pushButton_19"));

        horizontalLayout->addWidget(pushButton_19);

        pushButton_16 = new QPushButton(groupBox);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));

        horizontalLayout->addWidget(pushButton_16);

        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        horizontalLayout->addWidget(pushButton_5);

        pushButton_17 = new QPushButton(groupBox);
        pushButton_17->setObjectName(QStringLiteral("pushButton_17"));

        horizontalLayout->addWidget(pushButton_17);

        horizontalSpacer = new QSpacerItem(491, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        change = new QPushButton(groupBox);
        change->setObjectName(QStringLiteral("change"));

        horizontalLayout->addWidget(change);


        verticalLayout->addWidget(groupBox);


        verticalLayout_4->addWidget(toolW1);

        toolW2 = new QWidget(centralWidget);
        toolW2->setObjectName(QStringLiteral("toolW2"));
        verticalLayout_2 = new QVBoxLayout(toolW2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox_2 = new QGroupBox(toolW2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_9 = new QPushButton(groupBox_2);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        horizontalLayout_2->addWidget(pushButton_9);

        Index = new QLineEdit(groupBox_2);
        Index->setObjectName(QStringLiteral("Index"));
        Index->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_2->addWidget(Index);

        pushButton_7 = new QPushButton(groupBox_2);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        horizontalLayout_2->addWidget(pushButton_7);

        pushButton_12 = new QPushButton(groupBox_2);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));

        horizontalLayout_2->addWidget(pushButton_12);

        pushButton_13 = new QPushButton(groupBox_2);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));

        horizontalLayout_2->addWidget(pushButton_13);

        pushButton_8 = new QPushButton(groupBox_2);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        horizontalLayout_2->addWidget(pushButton_8);

        pushButton_20 = new QPushButton(groupBox_2);
        pushButton_20->setObjectName(QStringLiteral("pushButton_20"));

        horizontalLayout_2->addWidget(pushButton_20);

        horizontalSpacer_2 = new QSpacerItem(237, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout_2->addWidget(groupBox_2);


        verticalLayout_4->addWidget(toolW2);

        dataW = new QWidget(centralWidget);
        dataW->setObjectName(QStringLiteral("dataW"));
        dataW->setStyleSheet(QStringLiteral("border-color: rgb(0, 0, 255);"));
        verticalLayout_3 = new QVBoxLayout(dataW);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        widget_4 = new QWidget(dataW);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(widget_4);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        XMin = new QLineEdit(widget_4);
        XMin->setObjectName(QStringLiteral("XMin"));
        XMin->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_3->addWidget(XMin);

        XMax = new QLineEdit(widget_4);
        XMax->setObjectName(QStringLiteral("XMax"));
        XMax->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_3->addWidget(XMax);

        pushButton_10 = new QPushButton(widget_4);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_3->addWidget(pushButton_10);

        label_2 = new QLabel(widget_4);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        YMin = new QLineEdit(widget_4);
        YMin->setObjectName(QStringLiteral("YMin"));
        YMin->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_3->addWidget(YMin);

        YMax = new QLineEdit(widget_4);
        YMax->setObjectName(QStringLiteral("YMax"));
        YMax->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_3->addWidget(YMax);

        pushButton_14 = new QPushButton(widget_4);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));
        pushButton_14->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_3->addWidget(pushButton_14);

        pushButton_15 = new QPushButton(widget_4);
        pushButton_15->setObjectName(QStringLiteral("pushButton_15"));

        horizontalLayout_3->addWidget(pushButton_15);

        horizontalSpacer_3 = new QSpacerItem(707, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        handleCJL_btn = new QPushButton(widget_4);
        handleCJL_btn->setObjectName(QStringLiteral("handleCJL_btn"));

        horizontalLayout_3->addWidget(handleCJL_btn);

        pushButton_21 = new QPushButton(widget_4);
        pushButton_21->setObjectName(QStringLiteral("pushButton_21"));

        horizontalLayout_3->addWidget(pushButton_21);

        pushButton_11 = new QPushButton(widget_4);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));

        horizontalLayout_3->addWidget(pushButton_11);


        verticalLayout_3->addWidget(widget_4);

        plot = new QCustomPlot(dataW);
        plot->setObjectName(QStringLiteral("plot"));
        plot->setStyleSheet(QStringLiteral("border:1px solid blue;"));

        verticalLayout_3->addWidget(plot);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 10);

        verticalLayout_4->addWidget(dataW);

        verticalLayout_4->setStretch(0, 1);
        verticalLayout_4->setStretch(1, 1);
        verticalLayout_4->setStretch(2, 9);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setFloatable(true);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "\345\244\204\347\220\206\346\211\253\346\217\217\344\273\252\346\225\260\346\215\256", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "1", 0));
        pushButton_6->setText(QApplication::translate("MainWindow", "2", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "\345\220\210\345\271\266\345\211\224\351\231\244Y<0", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_4->setToolTip(QApplication::translate("MainWindow", "\345\225\246\345\225\246\345\225\246\345\225\246\345\225\246GG", 0));
#endif // QT_NO_TOOLTIP
        pushButton_4->setText(QApplication::translate("MainWindow", "\346\240\207\350\256\260\345\274\202\345\270\270\347\202\271", 0));
        pushButton_18->setText(QApplication::translate("MainWindow", "\350\256\241\347\256\227\347\227\205\345\256\263\345\214\272\345\237\237", 0));
        pushButton_19->setText(QApplication::translate("MainWindow", "\346\233\277\346\215\242\347\227\205\345\256\263\345\214\272\345\237\237", 0));
        pushButton_16->setText(QApplication::translate("MainWindow", "\350\241\245\345\267\246\345\217\263\347\253\226\347\272\277", 0));
        pushButton_5->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225", 0));
        pushButton_17->setText(QApplication::translate("MainWindow", "\345\206\231\345\205\245Excel", 0));
        change->setText(QApplication::translate("MainWindow", "\345\210\207\346\215\242", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\350\256\241\347\256\227\344\272\244\347\202\271", 0));
        pushButton_9->setText(QApplication::translate("MainWindow", "\350\216\267\345\217\226\347\233\270\346\234\272\345\217\202\346\225\260", 0));
        pushButton_7->setText(QApplication::translate("MainWindow", "\350\256\241\347\256\227\347\253\257\347\202\271\347\204\246\347\202\271\350\265\267\345\247\213\347\273\223\346\235\237\350\247\222\345\272\246", 0));
        pushButton_12->setText(QApplication::translate("MainWindow", "\350\256\241\347\256\227\346\237\220\347\233\270\346\234\272\347\204\246\347\202\271\345\257\271\345\272\224\346\211\200\346\234\211\346\225\260\346\215\256\347\202\271\350\247\222\345\272\246", 0));
        pushButton_13->setText(QApplication::translate("MainWindow", "\350\214\203\345\233\264\345\206\205\346\225\260\346\215\256\347\202\271\346\213\237\345\220\210\347\233\264\347\272\277\345\271\266\350\256\241\347\256\227\347\233\270\344\272\244\347\202\271", 0));
        pushButton_8->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\347\233\270\344\272\244\347\202\271", 0));
        pushButton_20->setText(QApplication::translate("MainWindow", "\350\216\267\345\217\226\350\243\201\345\211\252\345\203\217\347\264\240", 0));
        pushButton->setText(QApplication::translate("MainWindow", "\345\210\207\346\215\242\347\233\270\346\234\272", 0));
        label->setText(QApplication::translate("MainWindow", "X:", 0));
        pushButton_10->setText(QApplication::translate("MainWindow", "OK", 0));
        label_2->setText(QApplication::translate("MainWindow", "Y:", 0));
        pushButton_14->setText(QApplication::translate("MainWindow", "OK", 0));
        pushButton_15->setText(QApplication::translate("MainWindow", "ReDo", 0));
        handleCJL_btn->setText(QApplication::translate("MainWindow", "\346\265\213\350\267\235\350\275\256\345\244\204\347\220\206", 0));
        pushButton_21->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225\347\273\223\346\236\234", 0));
        pushButton_11->setText(QApplication::translate("MainWindow", "\346\213\274\346\216\245", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
