/**
 *  @file     task_queue.cc
 *  @brief    �^�X�N�L���[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/18
 *  @version  1.0
 */

 // include
#include "task/task_queue.h"
#include <vector>
#include <atomic>

namespace App
{
  namespace
  {
    /**
     *  @brief  �^�X�L���[�p�N���X
     */
    class TaskQueue final : public ITaskQueue
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      TaskQueue() : task_list_({}), current_index_(0)
      {

      }

      /**
       *  @brief  �f�X�g���N�^
       */
      ~TaskQueue()
      {
        task_list_.clear();
      }

      /**
       *  @brief  ���s�̏������s��
       */
      void MakeReady() override
      {
        // TODO:�^�X�N�̈ˑ��֌W���Ƀ\�[�g����

        current_index_ = 0;
      }

      /**
       *  @brief  �^�X�N��ǉ�����
       *  @param  task:�ǉ�����^�X�N
       */
      void Push(std::shared_ptr<ITask> task) override
      {
        task_list_.emplace_back(task);
      }
  
      /**
       *  @brief  �^�X�N���擾����
       *  @return �^�X�N�ւ̃V�F�A�[�h�|�C���^
       */
      std::shared_ptr<ITask> Pop() override
      {
        // �^�X�N�̈ˑ��֌W���Ƀ\�[�g����Ă��锤�Ȃ̂ŁA�擪������s���Ă���
        // �^�X�N�O���[�v���̃^�X�N���S�ďI��������A���̃^�X�N�O���[�v�̏������s���B
        // �^�X�N�O���[�v���̃^�X�N�̏I���҂����s��˂΂Ȃ�Ȃ�

        // current_index_��atomic�ϐ��Ȃ̂�
        // 1�̃^�X�N�������̃X���b�h�ňꏏ�Ɏ��s����鎖���N���Ȃ�
        std::uint32_t index = current_index_++;

        if (task_list_.size() <= index)
        {
          return nullptr;
        }

        return task_list_[index];
      }

      /**
       *  @brief  �L���[���N���A����
       */
      void Clear() override
      {
        task_list_.clear();
      }

    private:
      std::vector<std::shared_ptr<ITask>> task_list_; ///< �^�X�N�̃��X�g
      std::atomic<std::uint32_t> current_index_;      ///< �^�X�N���X�g�̉��Ԃ܂Ŏ��s�������̔ԍ�
    };
  };

  /**
   *  @brief  �^�X�N�L���[���쐬����
   *  @return �^�X�N�L���[�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<ITaskQueue> ITaskQueue::Create()
  {
    auto queue = std::make_shared<TaskQueue>();
    return queue;
  }
};