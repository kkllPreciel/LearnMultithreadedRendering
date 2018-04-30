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
#include <mutex>

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
      TaskQueue() : task_group_list_({}), executed_index_(0)
      {

      }

      /**
       *  @brief  �f�X�g���N�^
       */
      ~TaskQueue()
      {
        task_group_list_.clear();
      }

      /**
       *  @brief  �^�X�N�O���[�v��ǉ�����
       *  @param  task_group:�ǉ�����^�X�N�O���[�v
       */
      void Push(std::shared_ptr<ITaskGroup> task_group) override
      {
        // �^�X�N�O���[�v�̏I���C�x���g�ɓo�^����
        task_group->RegisterFinishEvent([&]() {
          ++executed_index_;
          condition_.notify_all();
        });
        task_group_list_.emplace_back(task_group);
      }
  
      /**
       *  @brief  �^�X�N���擾����
       *  @return �^�X�N�ւ̃V�F�A�[�h�|�C���^
       */
      std::shared_ptr<ITask> Pop() override
      {
        // ���s���ɒl���ς���Ă��܂��ꍇ������̂Ń��[�J���ϐ��ɃR�s�[����
        std::uint32_t executed_index = executed_index_;

        // �S�^�X�N�O���[�v�̑S�^�X�N���I������
        if (task_group_list_.size() <= executed_index)
        {
          return nullptr;
        }

        // ���s�\�ȃ^�X�N�O���[�v����^�X�N���擾����
        for (auto i = 0; i < task_group_list_.size(); ++i)
        {
          // �^�X�N���I���ς�
          if (task_group_list_[i]->Finished())
          {
            continue;
          }

          // ���s�\��Ԃł͂Ȃ�(�ˑ���̃^�X�N�O���[�v���I�����Ă��Ȃ�)
          if (false == task_group_list_[i]->Ready())
          {
            continue;
          }

          auto task = task_group_list_[i]->Pop();
          if (task != nullptr)
          {
            return task;
          }
        }

        // ���s�\�ȃ^�X�N�����݂��Ȃ��̂�
        // ���s���̃^�X�N�O���[�v���I������܂őҋ@���s��
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [&] { return executed_index != executed_index_; });

        return nullptr;
      }

    private:
      std::vector<std::shared_ptr<ITaskGroup>>  task_group_list_; ///< �^�X�N�O���[�v�̃��X�g
      std::atomic<std::uint32_t> executed_index_;                 ///< �^�X�N�O���[�v���X�g�̉��Ԃ܂Ŏ��s�������̔ԍ�
      std::mutex mutex_;                                          ///< �r������p�~���[�e�b�N�X
      std::condition_variable condition_;                         ///< ���s�ҋ@�p�����ϐ�
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