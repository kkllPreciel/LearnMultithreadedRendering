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
#include <mutex>

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
       *  @brief  ���s����
       *  @param  delta_time:�t���[���̌o�ߎ���
       */
      void Execute(std::uint64_t delta_time) override
      {
        delta_time_ = delta_time;

        // �X���b�h���N������
        condition_.notify_one();
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

          // �X���b�h���N������
          condition_.notify_one();

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
        while (true)
        {
          // �I���t���O�������Ă�����I������
          if (terminate_)
          {
            break;
          }

          std::shared_ptr<ITask> task = queue_->Pop();
          if (task != nullptr)
          {
            // �^�X�N�����s����
            task->Execute(delta_time_);
          }
          else if (false == queue_->Finished())
          {
            continue;
          }
          else
          {
            // �^�X�N�����݂��Ȃ��̂őҋ@����

            // �r������
            std::unique_lock<std::mutex> lk(mutex_);

            // �X���b�h�̑ҋ@
            condition_.wait(lk);
          }
        }
      }

    private:
      std::unique_ptr<std::thread> thread_; ///< �X���b�h
      std::shared_ptr<ITaskQueue> queue_;   ///< �^�X�N�L���[
      std::mutex mutex_;                    ///< �~���[�e�b�N�X
      std::condition_variable condition_;   ///< �����ϐ�
      std::uint64_t delta_time_;            ///< �t���[���̌o�ߎ���
      bool terminate_;                      ///< �I���t���O
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