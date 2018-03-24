#pragma once

#include <memory>
#include <QtWidgets/QWidget>
#include <qtimer.h>
#include "ui_learn_multithreaded_rendering.h"
#include "renderer/renderer.h"

class LearnMultithreadedRendering : public QWidget
{
  Q_OBJECT

public:
  /**
   *  @brief  �R���X�g���N�^
   *  @param  parent:�e�E�B�W�F�b�g
   */
  LearnMultithreadedRendering(QWidget *parent = Q_NULLPTR);

  /**
   *  @brief  �f�X�g���N�^
   */
  ~LearnMultithreadedRendering();

private Q_SLOTS:
  /**
   *  @brief  ���C�����[�v
   */
  void MainLoop(); 

private:
  Ui::LearnMultithreadedRenderingClass ui;
  std::unique_ptr<QTimer> timer;              ///< �^�C�}�[
  std::shared_ptr<App::IRenderer> renderer_;  ///< �����_���[
};
