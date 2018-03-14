#pragma once

#include <QtWidgets/QWidget>
#include "ui_learn_multithreaded_rendering.h"

class LearnMultithreadedRendering : public QWidget
{
    Q_OBJECT

public:
    LearnMultithreadedRendering(QWidget *parent = Q_NULLPTR);

private:
    Ui::LearnMultithreadedRenderingClass ui;
};
