/**
 *  @file     task_thread.h
 *  @brief    �^�X�N�X���b�h(���[�J�[�X���b�h)�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

#pragma once

// include
#include "task/task_queue.h"

namespace App
{
  /**
   *  @brief  �^�X�N�X���b�h�p�C���^�[�t�F�C�X
   */
  class ITaskThread
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    ITaskThread() = default;

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~ITaskThread() = default;

    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    ITaskThread(const ITaskThread& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    ITaskThread& operator = (const ITaskThread& other) = delete;

    /**
     *  @brief  ���s����
     *  @param  delta_time:�t���[���̌o�ߎ���
     */
    virtual void Execute(std::uint64_t delta_time) = 0;

    /**
     *  @brief  �I������
     */
    virtual void Destroy() = 0;

    /**
     *  @brief  �^�X�N�X���b�h���쐬����
     *  @param  queue:�^�X�N�̃L���[
     *  @return �^�X�N�X���b�h�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<ITaskThread> Create(std::shared_ptr<ITaskQueue> queue);
  };
};