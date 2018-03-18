#pragma once

#include <memory>
#include <QtWidgets/QWidget>
#include "ui_learn_multithreaded_rendering.h"
#include "renderer/renderer.h"

class LearnMultithreadedRendering : public QWidget
{
    Q_OBJECT

public:
  LearnMultithreadedRendering(QWidget *parent = Q_NULLPTR);
  ~LearnMultithreadedRendering();
  

private:
  Ui::LearnMultithreadedRenderingClass ui;
  std::shared_ptr<App::IRenderer> renderer_; ///< ƒŒƒ“ƒ_ƒ‰[
};
