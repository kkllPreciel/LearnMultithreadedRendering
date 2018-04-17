/**
 *  @file     task.h
 *  @brief    �^�X�N�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <functional>
#include <vector>

namespace App
{
  /**
   *  @brief  �^�X�N�p�C���^�[�t�F�C�X
   */
  class ITask
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    ITask() = default;

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~ITask() = default;

    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    ITask(const ITask& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    ITask& operator = (const ITask& other) = delete;

    /**
     *  @brief  �^�X�N�����s����
     *  @param  delta_time:�t���[���̌o�ߎ���
     */
    virtual void Execute(std::uint64_t delta_time) = 0;

    /**
     *  @brief  �ˑ��֌W�ɂ���^�X�N�̃��X�g���擾����
     *  @return �ˑ��֌W�ɂ���^�X�N�̃��X�g
     */
    virtual const std::vector<ITask*> GetDependenceTaskList() = 0;

    /**
     *  @brief  �^�X�N���I��������?
     *  @return �I���t���O
     */
    virtual bool Finished() const = 0;

    /**
     *  @brief  �^�X�N���쐬����
     *  @param  task_function:�^�X�N�֐�
     *  @param  dependence_task_list:�^�X�N���ˑ�����^�X�N�̃��X�g
     *  @return �^�X�N�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<ITask> Create(std::function<void(std::uint64_t delta_time)> task_function, const std::vector<ITask*>& dependence_task_list);
  };
};