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
#include <Sein/Direct3D12/vertex_buffer.h>
#include <Sein/Direct3D12/index_buffer.h>

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
     *  @brief  ���s����
     */
    virtual void Execute() = 0;

    /**
     *  @brief  ���_�o�b�t�@���쐬����
     *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
     *  @return ���_�o�b�t�@�ւ̃��j�[�N�|�C���^
     */
    virtual std::unique_ptr<Sein::Direct3D12::IVertexBuffer> CreateVertexBuffer(const std::uint32_t size_in_bytes) = 0;
    
    /**
     *  @brief  ���_�C���f�b�N�X�o�b�t�@���쐬����
     *  @param  size_in_bytes:���_�C���f�b�N�X�o�b�t�@�̃T�C�Y(���_�C���f�b�N�X�T�C�Y * ���_�C���f�b�N�X��)
     *  @return ���_�C���f�b�N�X�o�b�t�@�̃��j�[�NID
     */
    virtual std::unique_ptr<Sein::Direct3D12::IIndexBuffer> CreateIndexBuffer(const std::uint32_t size_in_bytes) = 0;

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
     *  @return �����_���[�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    static std::shared_ptr<IRenderer> Create(HWND handle, std::uint32_t width, std::uint32_t height);
  };
};