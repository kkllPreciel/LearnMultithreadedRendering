/**
 *  @file     graphics_pipeline_state.h
 *  @brief    �O���t�B�b�N�X�p�C�v���C���X�e�[�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/06/01
 *  @version  1.0
 */

#pragma once

// include
#include <memory>

#include <DirectXMath.h>
#include <Sein/Direct3D12/direct3d12_device.h>

namespace App
{
  /**
   *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g�p�C���^�[�t�F�C�X
   */
  class IGraphicsPipelineState
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    IGraphicsPipelineState() = default;
  
    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~IGraphicsPipelineState() = default;
  
    /**
     *  @brief  �I���������s��
     */
    virtual void Destroy() = 0;
  
    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    IGraphicsPipelineState(const IGraphicsPipelineState& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    IGraphicsPipelineState& operator = (const IGraphicsPipelineState& other) = delete;

    /**
     *  @brief  ���[���h�s��𕡐��ݒ肷��
     *  @param  world_matrices:���[���h�s��z��̐擪�|�C���^
     *  @param  size_in_bytes:�S���[���h�s��̃T�C�Y(���[���h�s��T�C�Y * ���[���h�s��)
     */
    virtual void SetWorldMatrices(const DirectX::XMFLOAT4X4* const world_matrices, const std::uint32_t size_in_bytes) = 0;

    /**
     *  @brief  �r���[�s���ݒ肷��
     *  @param  view_matrix:�r���[�s��
     */
    virtual void SetViewMatrix(const DirectX::XMFLOAT4X4& view_matrix) = 0;

    /**
     *  @brief  �v���W�F�N�V�����s���ݒ肷��
     *  @param  projection_matrix:�v���W�F�N�V�����s��
     */
    virtual void SetProjectionMatrix(const DirectX::XMFLOAT4X4& projection_matrix) = 0;

    /**
     *  @brief  �f�B�X�N���v�^�[�q�[�v���擾����
     *  @return �f�B�X�N���v�^�[�q�[�v
     */
    virtual std::shared_ptr<Sein::Direct3D12::IDescriptorHeap>& GetDescriptorHeap() = 0;

    /**
     *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g���쐬����
     *  @param  device:�f�o�C�X
     *  @return �O���t�B�b�N�X�p�C�v���C���X�e�[�g�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IGraphicsPipelineState> Create(Sein::Direct3D12::IDevice* const device);
  };
};