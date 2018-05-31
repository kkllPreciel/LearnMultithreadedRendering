/**
 *  @file     renderer.h
 *  @brief    �����_���[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/17
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <DirectXMath.h>
#include "model/mesh.h"
#include "task/task_scheduler.h"

namespace App
{
  /**
   *  @brief  �����_���[�p�C���^�[�t�F�C�X
   */
  class IRenderer
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    IRenderer() = default;
  
    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~IRenderer() = default;

    /**
     *  @brief  ���b�V�����쐬����
     *  @param  mesh_data:���b�V���f�[�^
     */
    virtual std::shared_ptr<IMesh> CreateMesh(std::shared_ptr<MeshLoader::IMeshData>& mesh_data) = 0;

    /**
     *  @brief  �`��I�u�W�F�N�g�̓o�^���s��
     *  @param  mesh:�`��I�u�W�F�N�g���g�p���郁�b�V��
     *  @param  matrix:�`��I�u�W�F�N�g�̃��[���h��ԍs��
     */
    virtual void Register(const std::shared_ptr<IMesh>& mesh, const DirectX::XMFLOAT4X4& matrix) = 0;

    /**
     *  @brief  �r���[��o�^����
     *  @param  view_id:�r���[��ID
     *  @param  view_matrix:�r���[�s��
     *  @param  projection_matrix:�v���W�F�N�V�����s��
     *  @param  viewport:�r���[�|�[�g��`�̐ݒ�
     *  @param  scissor:�V�U�[��`�̐ݒ�
     */
    virtual void RegisterView(const std::uint32_t view_id, const DirectX::XMFLOAT4X4& view_matrix, const DirectX::XMFLOAT4X4& projection_matrix, const D3D12_VIEWPORT viewport, const D3D12_RECT scissor) = 0;

    /**
     *  @brief  ���s����
     */
    virtual void Execute() = 0;

    /**
     *  @brief  ��ʂ��X�V����
     */
    virtual void Present() = 0;
  
    /**
     *  @brief  �I���������s��
     */
    virtual void Destroy() = 0;
  
    /**
     *  @brief  �R�s�[�R���X�g���N�^
     *  @param  other:�R�s�[���̃C���X�^���X
     */
    IRenderer(const IRenderer& other) = delete;
  
    /**
     *  @brief  ������Z�q�I�y���[�^
     *  @param  other:������̃C���X�^���X
     *  @return �����̃C���X�^���X
     */
    IRenderer& operator = (const IRenderer& other) = delete;

    /**
     *  @brief  �����_���[���쐬����
     *  @param  handle:�E�B���h�E�n���h��
     *  @param  width:�E�B���h�E�̉���
     *  @param  height:�E�B���h�E�̏c��
     *  @param  scheduler:�^�X�N�X�P�W���[��
     *  @return �����_���[�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IRenderer> Create(HWND handle, std::uint32_t width, std::uint32_t height, std::shared_ptr<ITaskScheduler> scheduler);
  };
};