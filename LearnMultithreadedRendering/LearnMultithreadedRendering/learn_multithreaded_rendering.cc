#include "learn_multithreaded_rendering.h"

/**
 *  @brief  �R���X�g���N�^
 *  @param  parent:�e�E�B�W�F�b�g
 */
LearnMultithreadedRendering::LearnMultithreadedRendering(QWidget *parent)
    : QWidget(parent), renderer_(nullptr)
{
    ui.setupUi(this);

    // �����_���[�̍쐬
    renderer_ = App::IRenderer::Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height());

    // ���C�����[�v�Ăяo���ݒ�
    timer = std::make_unique<QTimer>();
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(MainLoop()));
    timer->start(1000 / 60);
}

/**
 *  @brief  �f�X�g���N�^
 */
LearnMultithreadedRendering::~LearnMultithreadedRendering()
{
  if (timer->isActive())
  {
    timer->stop();
  }

  renderer_.reset();
}

/**
 *  @brief  ���C�����[�v
 */
void LearnMultithreadedRendering::MainLoop()
{
  // �񓯊��œ���������(�Q�[�����[�v�A�����_���[�AGPU)
  // �Q�[�����[�v�������_���[�փI�u�W�F�N�g���v�b�V��
  // ���̃t���[���Ń����_���[���R�}���h�o�b�t�@���쐬
  // ����Ɏ��̃t���[����GPU�������_�����O����ʂ��X�V�B

  // TODO:�����_�����O�J�n(�R�}���h�o�b�t�@���쐬���AGPU�ɏ����𓊂���)
  renderer_->Execute();

  // TODO:�Q�[���I�u�W�F�N�g�̍X�V

  // �����_�����O�I���҂�(GPU�̏����I���҂�)
  renderer_->Present();
}