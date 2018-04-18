/**
 *  @file     task_scheduler.cc
 *  @brief    �^�X�N�X�P�W���[���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/16
 *  @version  1.0
 */

 // include
#include "task/task_scheduler.h"
#include "task/task_queue.h"
#include "task/task_thread.h"

namespace App
{
  namespace
  {
    class TaskScheduler final : public ITaskScheduler
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      TaskScheduler() : queue_(nullptr), thread_list_({})
      {

      }

      /**
       *  @brief  �f�X�g���N�^
       */
      ~TaskScheduler()
      {
        Destroy();
      }

      /**
       *  @brief  �쐬����
       *  @param  num_threads:�g�p����X���b�h��
       */
      void Create(const std::uint32_t num_threads)
      {
        queue_ = ITaskQueue::Create();

        for (auto i = 0; i < num_threads; ++i)
        {
          thread_list_.emplace_back(ITaskThread::Create(queue_));
        }
      }

      /**
       *  @brief  �^�X�N��o�^����
       *  @param  task:�^�X�N
       */
      void Register(ITask* task) override
      {
        queue_->Push(task);
      }
      
      /**
       *  @brief  �^�X�N�����s����
       *  @param  delta_time:�t���[���̌o�ߎ���
       */
      void Execute(std::uint64_t delta_time) override
      {
        queue_->MakeReady();

        // �X���b�h�̋N��
        for (auto& thread : thread_list_)
        {
          thread->Execute(delta_time);
        }

        // �^�X�N�̎��s
        ITask* task = nullptr;
        while ((task = queue_->Pop()) != nullptr)
        {
          task->Execute(delta_time);
        }

        // TODO:�ʃX���b�h�Ń^�X�N�����s����������Ȃ��̂őҋ@����

        queue_->Clear();
      }
      
      /**
       *  @brief  �I������
       */
      void Destroy() override
      {
        // �X���b�h�̍폜
        for (auto& thread : thread_list_)
        {
          thread->Destroy();
        }
      }

    private:
      std::shared_ptr<ITaskQueue> queue_;                     ///< ���s����^�X�N�̃L���[
      std::vector<std::shared_ptr<ITaskThread>> thread_list_; ///< �^�X�N�X���b�h�̃��X�g
    };
  };

  /**
   *  @brief  �^�X�N�X�P�W���[�����쐬����
   *  @param  num_threads:�g�p����X���b�h��
   *  @return �^�X�N�X�P�W���[���C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<ITaskScheduler> ITaskScheduler::Create(const std::uint32_t num_threads)
  {
    auto task_scheduler = std::make_shared<TaskScheduler>();

    task_scheduler->Create(num_threads);

    return task_scheduler;
  }
};