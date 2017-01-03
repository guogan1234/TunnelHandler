/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *root_LE;
    QPushButton *start_btn;
    QSpacerItem *horizontalSpacer;
    QLineEdit *handleDir_LE;
    QPushButton *pushButton;
    QPushButton *change_btn;
    QLineEdit *dataIndex;
    QPushButton *pushButton_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QCustomPlot *plot;
    QWidget *widget_4;
    QGridLayout *gridLayout;
    QLineEdit *outTime_LE;
    QCheckBox *all_CB;
    QLineEdit *SDLen_LE;
    QLineEdit *minX_LE;
    QLabel *label;
    QPushButton *pushButton_5;
    QCheckBox *meanY_CB;
    QLineEdit *maxY_LE;
    QCheckBox *minX_CB;
    QLineEdit *inTime_LE;
    QLineEdit *meanY_LE;
    QCheckBox *maxY_CB;
    QPushButton *pushButton_3;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton_4;
    QFrame *line;
    QFrame *line_2;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(849, 595);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(Form);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        root_LE = new QLineEdit(widget);
        root_LE->setObjectName(QStringLiteral("root_LE"));
        root_LE->setMinimumSize(QSize(200, 0));
        root_LE->setReadOnly(true);

        horizontalLayout_2->addWidget(root_LE);

        start_btn = new QPushButton(widget);
        start_btn->setObjectName(QStringLiteral("start_btn"));

        horizontalLayout_2->addWidget(start_btn);

        horizontalSpacer = new QSpacerItem(240, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        handleDir_LE = new QLineEdit(widget);
        handleDir_LE->setObjectName(QStringLiteral("handleDir_LE"));
        handleDir_LE->setMinimumSize(QSize(250, 0));
        handleDir_LE->setReadOnly(true);

        horizontalLayout_2->addWidget(handleDir_LE);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        change_btn = new QPushButton(widget);
        change_btn->setObjectName(QStringLiteral("change_btn"));

        horizontalLayout_2->addWidget(change_btn);

        dataIndex = new QLineEdit(widget);
        dataIndex->setObjectName(QStringLiteral("dataIndex"));

        horizontalLayout_2->addWidget(dataIndex);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(Form);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        plot = new QCustomPlot(widget_2);
        plot->setObjectName(QStringLiteral("plot"));

        horizontalLayout->addWidget(plot);

        widget_4 = new QWidget(widget_2);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setMinimumSize(QSize(200, 0));
        gridLayout = new QGridLayout(widget_4);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        outTime_LE = new QLineEdit(widget_4);
        outTime_LE->setObjectName(QStringLiteral("outTime_LE"));

        gridLayout->addWidget(outTime_LE, 4, 1, 1, 1);

        all_CB = new QCheckBox(widget_4);
        all_CB->setObjectName(QStringLiteral("all_CB"));

        gridLayout->addWidget(all_CB, 3, 0, 1, 1);

        SDLen_LE = new QLineEdit(widget_4);
        SDLen_LE->setObjectName(QStringLiteral("SDLen_LE"));
        SDLen_LE->setReadOnly(true);

        gridLayout->addWidget(SDLen_LE, 7, 1, 1, 1);

        minX_LE = new QLineEdit(widget_4);
        minX_LE->setObjectName(QStringLiteral("minX_LE"));

        gridLayout->addWidget(minX_LE, 0, 1, 1, 1);

        label = new QLabel(widget_4);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 7, 0, 1, 1);

        pushButton_5 = new QPushButton(widget_4);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        gridLayout->addWidget(pushButton_5, 8, 1, 1, 1);

        meanY_CB = new QCheckBox(widget_4);
        meanY_CB->setObjectName(QStringLiteral("meanY_CB"));

        gridLayout->addWidget(meanY_CB, 2, 0, 1, 1);

        maxY_LE = new QLineEdit(widget_4);
        maxY_LE->setObjectName(QStringLiteral("maxY_LE"));

        gridLayout->addWidget(maxY_LE, 1, 1, 1, 1);

        minX_CB = new QCheckBox(widget_4);
        minX_CB->setObjectName(QStringLiteral("minX_CB"));

        gridLayout->addWidget(minX_CB, 0, 0, 1, 1);

        inTime_LE = new QLineEdit(widget_4);
        inTime_LE->setObjectName(QStringLiteral("inTime_LE"));
        inTime_LE->setReadOnly(true);

        gridLayout->addWidget(inTime_LE, 3, 1, 1, 1);

        meanY_LE = new QLineEdit(widget_4);
        meanY_LE->setObjectName(QStringLiteral("meanY_LE"));

        gridLayout->addWidget(meanY_LE, 2, 1, 1, 1);

        maxY_CB = new QCheckBox(widget_4);
        maxY_CB->setObjectName(QStringLiteral("maxY_CB"));

        gridLayout->addWidget(maxY_CB, 1, 0, 1, 1);

        pushButton_3 = new QPushButton(widget_4);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 5, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 9, 0, 1, 1);

        pushButton_4 = new QPushButton(widget_4);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        gridLayout->addWidget(pushButton_4, 5, 1, 1, 1);

        line = new QFrame(widget_4);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 6, 0, 1, 1);

        line_2 = new QFrame(widget_4);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 6, 1, 1, 1);


        horizontalLayout->addWidget(widget_4);

        horizontalLayout->setStretch(0, 20);

        verticalLayout->addWidget(widget_2);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        start_btn->setText(QApplication::translate("Form", "\351\200\211\346\213\251\347\233\256\345\275\225", 0));
        pushButton->setText(QApplication::translate("Form", "\345\210\207\346\215\242\346\225\260\346\215\256\345\272\223", 0));
        change_btn->setText(QApplication::translate("Form", "\345\210\207\346\215\242\344\270\213\344\270\200\347\216\257", 0));
        pushButton_2->setText(QApplication::translate("Form", "\350\267\263\350\275\254", 0));
        all_CB->setText(QApplication::translate("Form", "\346\230\257\345\220\246\346\273\241\350\266\263", 0));
        label->setText(QApplication::translate("Form", "\351\232\247\351\201\223\351\225\277\345\272\246\357\274\232", 0));
        pushButton_5->setText(QApplication::translate("Form", "\346\265\213\350\267\235\350\275\256\346\225\260\346\215\256\350\256\241\347\256\227", 0));
        meanY_CB->setText(QApplication::translate("Form", "mean(y)>3", 0));
        minX_CB->setText(QApplication::translate("Form", "Min(x)>-12", 0));
        maxY_CB->setText(QApplication::translate("Form", "Max(y)<7", 0));
        pushButton_3->setText(QApplication::translate("Form", "\350\207\252\345\212\250\351\234\200\346\211\276\350\277\233\346\264\236", 0));
        pushButton_4->setText(QApplication::translate("Form", "\350\207\252\345\212\250\351\234\200\346\211\276\345\207\272\346\264\236", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
