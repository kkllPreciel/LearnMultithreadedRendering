/**
 *  @file     task.cc
 *  @brief    �^�X�N�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

 // include
#include "task/task.h"

namespace App
{
  namespace
  {
    /**
     *  @brief  �^�X�N�p�N���X
     */
    class Task final : public ITask
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       *  @param  task:�^�X�N�֐�
       *  @param  dependence_task_list:�^�X�N���ˑ�����^�X�N�̃��X�g
       */
      Task(std::function<void(std::uint64_t)> task, std::vector<ITask*> dependence_task_list) : task_(task), dependence_task_list_(dependence_task_list), finished_(false)
      {

      }

      /**
       *  @brief  �f�X�g���N�^
       */
      ~Task()
      {

      }

      /**
       *  @brief  �^�X�N�����s����
       *  @param  delta_time:�t���[���̌o�ߎ���
       */
      void Execute(std::uint64_t delta_time) override
      {
        for (auto task : dependence_task_list_)
        {
          if (task->Finished())
          {
            continue;
          }

          // TODO:�����Ɏ��s���Ă����2����s����Ă��܂�
          task->Execute(delta_time);
        }

        task_(delta_time);
        
        finished_ = true;
      }

      /**
       *  @brief  �ˑ��֌W�ɂ���^�X�N�̃��X�g���擾����
       *  @return �ˑ��֌W�ɂ���^�X�N�̃��X�g
       */
      const std::vector<ITask*> GetDependenceTaskList() override
      {
        return dependence_task_list_;
      }

      /**
       *  @brief  �^�X�N���I��������?
       *  @return �I���t���O
       */
      bool Finished() const override
      {
        return finished_;
      }

    private:
      std::function<void(std::uint64_t)> task_;   ///< �^�X�N�֐�
      std::vector<ITask*> dependence_task_list_;  ///< ���̃^�X�N���ˑ�����^�X�N�̃��X�g
      bool finished_;                             ///< �I���ς݃t���O
    };
  };

  /**
   *  @brief  �^�X�N���쐬����
   *  @param  task_function:�^�X�N�֐�
   *  @param  dependence_task_list:�^�X�N���ˑ�����^�X�N�̃��X�g
   *  @return �^�X�N�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<ITask> ITask::Create(std::function<void(std::uint64_t delta_time)> task_function, const std::vector<ITask*>& dependence_task_list)
  {
    auto task = std::make_shared<Task>(task_function, dependence_task_list);
    return task;
  }
};