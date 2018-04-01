/********************************************************************************
** Form generated from reading UI file 'learn_multithreaded_rendering.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEARN_MULTITHREADED_RENDERING_H
#define UI_LEARN_MULTITHREADED_RENDERING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LearnMultithreadedRenderingClass
{
public:

    void setupUi(QWidget *LearnMultithreadedRenderingClass)
    {
        if (LearnMultithreadedRenderingClass->objectName().isEmpty())
            LearnMultithreadedRenderingClass->setObjectName(QStringLiteral("LearnMultithreadedRenderingClass"));
        LearnMultithreadedRenderingClass->resize(600, 400);

        retranslateUi(LearnMultithreadedRenderingClass);

        QMetaObject::connectSlotsByName(LearnMultithreadedRenderingClass);
    } // setupUi

    void retranslateUi(QWidget *LearnMultithreadedRenderingClass)
    {
        LearnMultithreadedRenderingClass->setWindowTitle(QApplication::translate("LearnMultithreadedRenderingClass", "LearnMultithreadedRendering", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LearnMultithreadedRenderingClass: public Ui_LearnMultithreadedRenderingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEARN_MULTITHREADED_RENDERING_H
