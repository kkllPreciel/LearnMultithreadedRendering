/**
 *  @file     task_group.cc
 *  @brief    �^�X�N�O���[�v�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/04/20
 *  @version  1.0
 */

 // include
#include "task/task_group.h"

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
        : task_list_(task_list), dependence_task_group_list_(dependence_task_group_list)
      {

      }

      /**
       *  @brief  �f�X�g���N�^
       */
      ~TaskGroup() override
      {

      }

      /**
       *  @brief  �^�X�N�O���[�v�Ɋ܂܂��^�X�N�̌����擾����
       *  @return �^�X�N�̌�
       */
      const std::uint32_t GetTaskCount() const override
      {
        return task_list_.size();
      }
      
      /**
       *  @brief  �w�肳�ꂽ�ԍ��̃^�X�N���擾����
       *  @param  index:�^�X�N�̔ԍ�
       *  @return �^�X�N�ւ̃V�F�A�[�h�|�C���^
       */
      std::shared_ptr<ITask> GetTask(const std::uint32_t index) const override
      {
        return task_list_[index];
      }

    private:
      std::vector<std::shared_ptr<ITask>> task_list_;                         ///< �^�X�N�O���[�v�ɑ�����^�X�N�̃��X�g
      std::vector<std::shared_ptr<ITaskGroup>>  dependence_task_group_list_;  ///< �ˑ���̃^�X�N�O���[�v�̃��X�g
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