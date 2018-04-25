/**
 *  @file     task_group.h
 *  @brief    �^�X�N�O���[�v�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/04/20
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
#include "task/task.h"

namespace App
{
  /**
   *  @brief  �^�X�N�O���[�v�p�C���^�[�t�F�C�X
   */
  class ITaskGroup
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    ITaskGroup() = default;

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~ITaskGroup() = default;

    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    ITaskGroup(const ITaskGroup& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    ITaskGroup& operator = (const ITaskGroup& other) = delete;

    /**
     *  @brief  �^�X�N�O���[�v�����s�\��?
     *  @return ���s�\�t���O
     */
    virtual bool Ready() = 0;

    /**
     *  @brief  �^�X�N���擾����
     *  @return �^�X�N�ւ̃V�F�A�[�h�|�C���^
     */
    virtual std::shared_ptr<ITask> Pop() = 0;

    /**
     *  @brief  �^�X�N�O���[�v���̑S�^�X�N���I��������?
     *  @return �I���t���O
     */
    virtual bool Finished() = 0;

    /**
     *  @brief  �I���҂����s��
     */
    virtual void Wait() = 0;

    /**
     *  @brief  �^�X�N�O���[�v�̑S�^�X�N�I�����̃C�x���g�ɓo�^����
     *  @param  function:�^�X�N�O���[�v�̑S�^�X�N�I�����Ɏ��s����֐�
     */
    virtual void RegisterFinishEvent(std::function<void()> function) = 0;

    /**
     *  @brief  �^�X�N�O���[�v���쐬����
     *  @param  task_list:�^�X�N�̃��X�g
     *  @param  dependence_task_group_list:�^�X�N�O���[�v���ˑ�����^�X�N�O���[�v�̃��X�g
     *  @return �^�X�N�O���[�v�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<ITaskGroup> Create(std::vector<std::shared_ptr<ITask>>& task_list, std::vector<std::shared_ptr<ITaskGroup>>& dependence_task_group_list);
  };
};