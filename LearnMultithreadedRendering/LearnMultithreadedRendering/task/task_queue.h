/**
 *  @file     task_queue.h
 *  @brief    �^�X�N�L���[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/04/18
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include "task/task.h"
#include "task/task_group.h"

namespace App
{
  /**
   *  @brief  �^�X�N�L���[�p�C���^�[�t�F�C�X
   */
  class ITaskQueue
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    ITaskQueue() = default;

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~ITaskQueue() = default;

    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    ITaskQueue(const ITaskQueue& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    ITaskQueue& operator = (const ITaskQueue& other) = delete;

    /**
     *  @brief  �^�X�N�O���[�v��ǉ�����
     *  @param  task_group:�ǉ�����^�X�N�O���[�v
     */
    virtual void Push(std::shared_ptr<ITaskGroup> task_group) = 0;

    /**
     *  @brief  �^�X�N���擾����
     *  @return �^�X�N�ւ̃V�F�A�[�h�|�C���^
     */
    virtual std::shared_ptr<ITask> Pop() = 0;

    /**
     *  @brief  �L���[���̑S�^�X�N�O���[�v���I��������?
     *  @return �I���t���O
     */
    virtual bool Finished() = 0;

    /**
     *  @brief  �^�X�N�L���[���쐬����
     *  @return �^�X�N�L���[�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<ITaskQueue> Create();
  };
};