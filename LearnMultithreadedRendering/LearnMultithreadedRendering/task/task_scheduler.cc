/**
 *  @file     task_scheduler.cc
 *  @brief    �^�X�N�X�P�W���[���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/16
 *  @version  1.0
 */

 // include
#include "task_scheduler.h"
#include <deque>

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
      TaskScheduler() : task_deque_()
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
       *  @brief  �^�X�N��o�^����
       *  @param  task:�^�X�N�֐�
       */
      void Register(std::function<void(std::uint64_t delta_time)> task) override
      {
        task_deque_.emplace_back(task);
      }
      
      /**
       *  @brief  �^�X�N�����s����
       *  @param  delta_time:�t���[���̌o�ߎ���
       */
      void Execute(std::uint64_t delta_time) override
      {
        for (decltype(auto) task : task_deque_)
        {
          task(delta_time);
        }
      }
      
      /**
       *  @brief  �I������
       */
      void Destroy() override
      {
        task_deque_.clear();
      }

    private:
      std::deque<std::function<void(std::uint64_t)>> task_deque_; ///< ���s����^�X�N��deque
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
    return task_scheduler;
  }
};