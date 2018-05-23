/**
 *  @file     double_command_list.h
 *  @brief    �_�u���R�}���h���X�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/04/15
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <Sein/Direct3D12/command_list.h>
#include <Sein/Direct3D12/direct3d12_device.h>

namespace App
{
  /**
   *  @brief  �_�u���o�b�t�@�����O���s���R�}���h���X�g�p�C���^�[�t�F�C�X
   */
  class IDoubleCommandList
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    IDoubleCommandList() = default;
  
    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~IDoubleCommandList() = default;

    /**
     *  @brief  �R�}���h���X�g�̌������s��
     */
    virtual void Swap() = 0;

    /**
     *  @brief  ���݃t�����g�̃R�}���h���X�g���擾����
     *  @return �R�}���h���X�g�ւ̎Q��
     */
    virtual const Sein::Direct3D12::ICommandList& GetFrontCommandList() = 0;
    
    /**
     *  @brief  ���݃o�b�N�̃R�}���h���X�g���擾����
     *  @return �R�}���h���X�g�ւ̎Q��
     */
    virtual Sein::Direct3D12::ICommandList& GetBackCommandList() = 0;
  
    /**
     *  @brief  �I���������s��
     */
    virtual void Destroy() = 0;
  
    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    IDoubleCommandList(const IDoubleCommandList& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    IDoubleCommandList& operator = (const IDoubleCommandList& other) = delete;

    /**
     *  @brief  �_�u���R�}���h���X�g���쐬����
     *  @param  device:�f�o�C�X
     *  @param  command_list_type:�R�}���h���X�g�̃^�C�v
     *  @return �_�u���R�}���h���X�g�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IDoubleCommandList> Create(std::shared_ptr<Sein::Direct3D12::IDevice> device, const D3D12_COMMAND_LIST_TYPE& command_list_type);
  };
};