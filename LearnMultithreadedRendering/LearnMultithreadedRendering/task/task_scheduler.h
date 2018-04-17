/**
 *  @file     task_scheduler.h
 *  @brief    �^�X�N�X�P�W���[���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/04/16
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <functional>
#include "task/task.h"

namespace App
{
  /**
   *  @brief  �^�X�N�X�P�W���[���p�C���^�[�t�F�C�X
   */
  class ITaskScheduler
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    ITaskScheduler() = default;

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~ITaskScheduler() = default;

    /**
     *  @brief  �^�X�N��o�^����
     *  @param  task:�^�X�N
     */
    virtual void Register(ITask* task) = 0;

    /**
     *  @brief  �^�X�N�����s����
     *  @param  delta_time:�t���[���̌o�ߎ���
     */
    virtual void Execute(std::uint64_t delta_time) = 0;

    /**
     *  @brief  �I������
     */
    virtual void Destroy() = 0;

    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    ITaskScheduler(const ITaskScheduler& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    ITaskScheduler& operator = (const ITaskScheduler& other) = delete;

    /**
     *  @brief  �^�X�N�X�P�W���[�����쐬����
     *  @param  num_threads:�g�p����X���b�h��
     *  @return �^�X�N�X�P�W���[���C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<ITaskScheduler> Create(const std::uint32_t num_threads);
  };
};