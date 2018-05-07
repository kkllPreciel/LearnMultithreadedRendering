#pragma once

#include <memory>
#include <QtWidgets/QWidget>
#include <qtimer.h>
#include "ui_learn_multithreaded_rendering.h"
#include "renderer/renderer.h"
#include "task/task_scheduler.h"

class LearnMultithreadedRendering : public QWidget
{
  Q_OBJECT

public:
  /**
   *  @brief  コンストラクタ
   *  @param  parent:親ウィジェット
   */
  LearnMultithreadedRendering(QWidget *parent = Q_NULLPTR);

  /**
   *  @brief  デストラクタ
   */
  ~LearnMultithreadedRendering();

private Q_SLOTS:
  /**
   *  @brief  メインループ
   */
  void MainLoop(); 

private:
  Ui::LearnMultithreadedRenderingClass ui;
  std::unique_ptr<QTimer> timer;                    ///< タイマー
  std::shared_ptr<App::IRenderer> renderer_;        ///< レンダラー
  std::shared_ptr<App::ITaskScheduler> scheduler_;  ///< スケジューラ
};
