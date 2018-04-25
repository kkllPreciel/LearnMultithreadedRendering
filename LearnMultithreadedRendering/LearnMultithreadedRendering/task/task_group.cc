/**
 *  @file     task_group.cc
 *  @brief    �^�X�N�O���[�v�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/20
 *  @version  1.0
 */

 // include
#include "task/task_group.h"
#include <atomic>
#include <mutex>

namespace App
{
  namespace
  {
    /**
     *  @brief  �^�X�N�O���[�v�p�N���X
     */
    class TaskGroup final : public ITaskGroup
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       *  @param  task_list:�^�X�N�O���[�v�ɑ�����^�X�N�̃��X�g
       *  @param  dependence_task_group_list:�ˑ���̃^�X�N�O���[�v�̃��X�g
       */
      TaskGroup(std::vector<std::shared_ptr<ITask>>& task_list, std::vector<std::shared_ptr<ITaskGroup>>& dependence_task_group_list)
        : task_list_(task_list), dependence_task_group_list_(dependence_task_group_list), executed_index_(0), finished_count_(0), finished_(false), callback_list_({})
      {
        // �^�X�N�̏I���C�x���g�ɓo�^����
        for (auto task : task_list_)
        {
          task->RegisterFinishEvent([&](){
            if (++finished_count_ == task_list_.size())
            {
              std::unique_lock<std::mutex> lock(mutex_);
              finished_ = true;

              // �I���C�x���g��ʒm����
              for (auto callback : callback_list_)
              {
                callback();
              }

              condition_.notify_all();
            }
          });
        }
      }

      /**
       *  @brief  �f�X�g���N�^
       */
      ~TaskGroup() override
      {

      }

      /**
       *  @brief  �^�X�N�O���[�v�����s�\��?
       *  @return ���s�\�t���O
       */
      bool Ready() override
      {
        for (auto dependence_task_group : dependence_task_group_list_)
        {
          if (dependence_task_group->Finished() == false)
          {
            return false;
          }
        }

        return true;
      }

      /**
       *  @brief  �^�X�N���擾����
       *  @return �^�X�N�ւ̃V�F�A�[�h�|�C���^
       */
      std::shared_ptr<ITask> Pop() override
      {
        // �ˑ���̃^�X�N�O���[�v���I�����Ă��Ȃ����
        // �ˑ���̃^�X�N�O���[�v�̃^�X�N��߂�
        for (auto dependence_task_group : dependence_task_group_list_)
        {
          if (dependence_task_group->Finished() == false)
          {
            auto dependence_task = dependence_task_group->Pop();
            if (dependence_task != nullptr)
            {
              return dependence_task;
            }

            // �ˑ���̃^�X�N�O���[�v�̃^�X�N���擾�ł��Ȃ��ꍇ��
            // �ˑ���̃^�X�N�O���[�v�̏I���҂����s��
            dependence_task_group->Wait();
          }
        }

        // executed_index_��atomic�ϐ��Ȃ̂�
        // 1�̃^�X�N�������̃X���b�h�ňꏏ�Ɏ��s����鎖���N���Ȃ�
        std::uint32_t index = executed_index_++;
        if (task_list_.size() <= index)
        {
          return nullptr;
        }

        return task_list_[index];
      }

      /**
       *  @brief  �I���҂����s��
       */
      void Wait() override
      {
        std::unique_lock<std::mutex> lock(mutex_);
        if (finished_ == false)
        {
          condition_.wait(lock, [&]{ return finished_; });
        }
      }

      /**
       *  @brief  �^�X�N�O���[�v���̑S�^�X�N���I��������?
       *  @return �I���t���O
       */
      bool Finished() override
      {
        return finished_;
      }

      /**
       *  @brief  �^�X�N�O���[�v�̑S�^�X�N�I�����̃C�x���g�ɓo�^����
       *  @param  function:�^�X�N�O���[�v�̑S�^�X�N�I�����Ɏ��s����֐�
       */
      void RegisterFinishEvent(std::function<void()> function) override
      {
        callback_list_.emplace_back(function);
      }

    private:
      std::vector<std::shared_ptr<ITask>> task_list_;                         ///< �^�X�N�O���[�v�ɑ�����^�X�N�̃��X�g
      std::atomic<std::uint32_t> executed_index_;                             ///< �^�X�N���X�g�̉��Ԃ܂Ŏ��s�������̔ԍ�
      std::atomic<std::uint32_t> finished_count_;                             ///< ���s���I�������^�X�N�̌�
      bool finished_;                                                         ///< �S�����I���ς݃t���O
      std::vector<std::shared_ptr<ITaskGroup>>  dependence_task_group_list_;  ///< �ˑ���̃^�X�N�O���[�v�̃��X�g
      std::mutex mutex_;                                                      ///< �r������p�~���[�e�b�N�X
      std::condition_variable condition_;                                     ///< ���s�ҋ@�p�����ϐ�
      std::vector<std::function<void()>> callback_list_;                      ///< �^�X�N�O���[�v���̑S�^�X�N�I�����Ɏ��s����֐��̃��X�g
    };
  };

  /**
   *  @brief  �^�X�N�O���[�v���쐬����
   *  @param  task_list:�^�X�N�̃��X�g
   *  @param  dependence_task_group_list:�^�X�N�O���[�v���ˑ�����^�X�N�O���[�v�̃��X�g
   *  @return �^�X�N�O���[�v�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<ITaskGroup> ITaskGroup::Create(std::vector<std::shared_ptr<ITask>>& task_list, std::vector<std::shared_ptr<ITaskGroup>>& dependence_task_group_list)
  {
    auto task_group = std::make_shared<TaskGroup>(task_list, dependence_task_group_list);
    return task_group;
  }
};