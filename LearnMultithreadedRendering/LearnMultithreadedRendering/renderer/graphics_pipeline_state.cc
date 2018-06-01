/**
 *  @file     graphics_pipeline_state.cc
 *  @brief    �O���t�B�b�N�X�p�C�v���C���X�e�[�g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/01
 *  @version  1.0
 */

 // include
#include "renderer/graphics_pipeline_state.h"

namespace App
{
  namespace
  {
    /**
     *  @brief  �萔�o�b�t�@�p�N���X
     */
    class ConstantBuffer
    {
    public:
#pragma pack(push, 1)
      DirectX::XMFLOAT4X4 world_;
      DirectX::XMFLOAT4X4 view_;
      DirectX::XMFLOAT4X4 projection_;
#pragma pack(pop)
    };

    /**
     *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g�p�N���X
     */
    class GraphicsPipelineState final : public IGraphicsPipelineState
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      GraphicsPipelineState()
      {

      }
    
      /**
       *  @brief  �f�X�g���N�^
       */
      ~GraphicsPipelineState() override
      {
        Destroy();
      }

      /**
       *  @brief  �p�C�v���C���X�e�[�g���쐬����
       *  @param  device:�f�o�C�X
       */
      void Create(Sein::Direct3D12::IDevice* const device)
      {
        // �f�B�X�N���v�^�[�q�[�v�̍쐬
        D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc = {};
        descriptor_heap_desc.NumDescriptors = 5;                                 // �f�B�X�N���v�^�[�q�[�v���̃f�B�X�N���v�^�[��(�萔�o�b�t�@�A�V�F�[�_�[���\�[�X)
        descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;      // �萔�o�b�t�@ or �V�F�[�_�[���\�[�X(�e�N�X�`��) or �����_���A�N�Z�X �̂ǂꂩ�̃q�[�v
        descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  // �V�F�[�_�[����A�N�Z�X��
        descriptor_heap_ = device->CreateDescriptorHeap(descriptor_heap_desc);

        // �萔�o�b�t�@�̍쐬
        constant_buffer_ = device->CreateConstantBuffer(descriptor_heap_, sizeof(ConstantBuffer));
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.world_), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.view_), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.projection_), DirectX::XMMatrixIdentity());

        // �V�F�[�_�[���\�[�X�o�b�t�@(StructuredBuffer)�̍쐬
        // TODO:1�v�f�̃T�C�Y�Ɨv�f�����w��ł���悤�ɂ���
        resource_buffer_ = device->CreateShaderResourceBuffer(descriptor_heap_, 10000, sizeof(DirectX::XMFLOAT4X4));
      }

      /**
       *  @brief  ���[���h�s��𕡐��ݒ肷��
       *  @param  world_matrices:���[���h�s��z��̐擪�|�C���^
       *  @param  size_in_bytes:�S���[���h�s��̃T�C�Y(���[���h�s��T�C�Y * ���[���h�s��)
       */
      void SetWorldMatrices(const DirectX::XMFLOAT4X4* const world_matrices, const std::uint32_t size_in_bytes) override
      {
        resource_buffer_->Map(world_matrices, size_in_bytes);
      }

      /**
       *  @brief  �r���[�s���ݒ肷��
       *  @param  view_matrix:�r���[�s��
       */
      void SetViewMatrix(const DirectX::XMFLOAT4X4& view_matrix) override
      {
        constant_buffer_instance_.view_ = view_matrix;
        constant_buffer_->Map(sizeof(ConstantBuffer), &constant_buffer_instance_);
      }
      
      /**
       *  @brief  �v���W�F�N�V�����s���ݒ肷��
       *  @param  projection_matrix:�v���W�F�N�V�����s��
       */
      void SetProjectionMatrix(const DirectX::XMFLOAT4X4& projection_matrix) override
      {
        constant_buffer_instance_.projection_ = projection_matrix;
        constant_buffer_->Map(sizeof(ConstantBuffer), &constant_buffer_instance_);
      }

      /**
       *  @brief  �f�B�X�N���v�^�[�q�[�v���擾����
       *  @return �f�B�X�N���v�^�[�q�[�v
       */
      std::shared_ptr<Sein::Direct3D12::IDescriptorHeap>& GetDescriptorHeap() override
      {
        return descriptor_heap_;
      }
    
      /**
       *  @brief  �I���������s��
       */
      void Destroy() override
      {

      }

    private:
      std::shared_ptr<Sein::Direct3D12::IDescriptorHeap> descriptor_heap_;      ///< �萔�o�b�t�@�p�f�B�X�N���v�^�[�q�[�v
      ConstantBuffer constant_buffer_instance_;                                 ///< �R���X�^���g�o�b�t�@�̎���
      std::unique_ptr<Sein::Direct3D12::IConstantBuffer> constant_buffer_;      ///< �萔�o�b�t�@
      std::vector<DirectX::XMFLOAT4X4> instance_buffer_;                        ///< �C���X�^���X�o�b�t�@�̎���
      std::unique_ptr<Sein::Direct3D12::ShaderResourceBuffer> resource_buffer_; ///< ���[���h�s��pStructuredBuffer
    };
  };

  /**
   *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g���쐬����
   *  @param  device:�f�o�C�X
   *  @return �O���t�B�b�N�X�p�C�v���C���X�e�[�g�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IGraphicsPipelineState> IGraphicsPipelineState::Create(Sein::Direct3D12::IDevice* const device)
  {
    auto graphics_pipeline_state = std::make_shared<GraphicsPipelineState>();

    graphics_pipeline_state->Create(device);

    return graphics_pipeline_state;
  }
};