/********************************************************************************
** Form generated from reading UI file 'PowerPlot.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POWERPLOT_H
#define UI_POWERPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PowerPlot
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *freq;

    void setupUi(QWidget *PowerPlot)
    {
        if (PowerPlot->objectName().isEmpty())
            PowerPlot->setObjectName(QStringLiteral("PowerPlot"));
        PowerPlot->resize(400, 300);
        verticalLayout = new QVBoxLayout(PowerPlot);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        freq = new QLabel(PowerPlot);
        freq->setObjectName(QStringLiteral("freq"));
        freq->setText(QStringLiteral(""));

        verticalLayout->addWidget(freq, 0, Qt::AlignRight|Qt::AlignTop);


        retranslateUi(PowerPlot);

        QMetaObject::connectSlotsByName(PowerPlot);
    } // setupUi

    void retranslateUi(QWidget *PowerPlot)
    {
        PowerPlot->setWindowTitle(QApplication::translate("PowerPlot", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class PowerPlot: public Ui_PowerPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POWERPLOT_H
