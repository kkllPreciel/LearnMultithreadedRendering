/**
 *  @file     task.cc
 *  @brief    �^�X�N�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

 // include
#include "task/task.h"
#include <cassert>
#include <atomic>
#include <vector>

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
       */
      Task(std::function<void(std::uint64_t)> task) : task_(task), finished_(false), executing_(false), callback_list_({})
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
        assert(!executing_ && !finished_);
        executing_ = true;

        task_(delta_time);
        
        executing_ = false;
        finished_ = true;

        // �I���C�x���g��ʒm����
        for (auto callback : callback_list_)
        {
          callback();
        }
      }

      /**
       *  @brief  �^�X�N�I�����̃C�x���g�ɓo�^����
       *  @param  function:�^�X�N�I�����Ɏ��s����֐�
       */
      void RegisterFinishEvent(std::function<void()> function) override
      {
        callback_list_.emplace_back(function);
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
      std::function<void(std::uint64_t)> task_;           ///< �^�X�N�֐�
      bool finished_;                                     ///< �I���ς݃t���O
      bool executing_;                                    ///< ���s���t���O
      std::vector<std::function<void()>> callback_list_;  ///< �^�X�N�I�����Ɏ��s����֐��̃��X�g
    };
  };

  /**
   *  @brief  �^�X�N���쐬����
   *  @param  task_function:�^�X�N�֐�
   *  @return �^�X�N�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<ITask> ITask::Create(std::function<void(std::uint64_t delta_time)> task_function)
  {
    auto task = std::make_shared<Task>(task_function);
    return task;
  }
};