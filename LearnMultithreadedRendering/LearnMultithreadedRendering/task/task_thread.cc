/**
 *  @file     task_thread.cc
 *  @brief    �^�X�N�X���b�h(���[�J�[�X���b�h)�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

 // include
#include "task/task_thread.h"
#include <thread>
#include <atomic>

namespace App
{
  namespace
  {
    /**
     *  @brief  �^�X�N�X���b�h�p�N���X
     */
    class TaskThread final : public ITaskThread
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      TaskThread() : thread_(nullptr), queue_(), terminate_(false)
      {

      }

      /**
       *  @brief  �f�X�g���N�^
       */
      ~TaskThread() override
      {
        Destroy();
      }

      /**
       *  @brief  �쐬����
       *  @param  queue:�^�X�N�L���[
       */
      void Create(std::shared_ptr<ITaskQueue> queue)
      {
        queue_ = queue;
        thread_ = std::make_unique<std::thread>(&TaskThread::Run, this);
      }
  
      /**
       *  @brief  �I������
       */
      void Destroy() override
      {
        if (thread_ && thread_->joinable())
        {
          // �I���t���O�𗧂Ă�
          terminate_ = true;

          // �X���b�h�̏I���҂����s��
          thread_->join();

          thread_.reset();
        }
      }

    private:
      /**
       *  @brief  �^�X�N�X���b�h�̃��C���֐�
       */
      void Run()
      {
        while (terminate_ == false)
        {
          std::shared_ptr<ITask> task = queue_->Pop();

          // TODO:�����ϐ����g���đҋ@���������ǂ������ׂ�(CPU�����ʂɉ��̂�)
          if (task == nullptr)
          {
            continue;
          }

          // �^�X�N�����s����
          task->Execute();
        }
      }

    private:
      std::unique_ptr<std::thread> thread_; ///< �X���b�h
      std::shared_ptr<ITaskQueue> queue_;   ///< �^�X�N�L���[
      std::atomic<bool> terminate_;         ///< �I���t���O
    };
  };

  /**
   *  @brief  �^�X�N�X���b�h���쐬����
   *  @param  queue:�^�X�N�̃L���[
   *  @return �^�X�N�X���b�h�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<ITaskThread> ITaskThread::Create(std::shared_ptr<ITaskQueue> queue)
  {
    auto task_thread = std::make_shared<TaskThread>();
    
    task_thread->Create(queue);

    return task_thread;
  }
};