/**
 *  @file     renderer.cc
 *  @brief    �����_���[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/17
 *  @version  1.0
 */

 // include
#include <Sein/Direct3D12/direct3d12_device.h>
#include "renderer/renderer.h"

namespace App
{
  namespace
  {
    /**
     *  @brief  �����_���[�p�N���X
     */
    class Renderer final : public IRenderer
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Renderer() : device_(nullptr), execute_command_list_(nullptr), store_command_list_(nullptr)
      {

      }
    
      /**
       *  @brief  �f�X�g���N�^
       */
      ~Renderer() override
      {
        Destroy();
      }

      /**
       *  @brief  �f�o�C�X���쐬����
       *  @param  handle:�E�B���h�E�n���h��
       *  @param  width:�E�B���h�E�̉���
       *  @param  height:�E�B���h�E�̏c��
       */
      void Create(HWND handle, std::uint32_t width, std::uint32_t height)
      {
        assert(device_ == nullptr);
        device_ = std::make_unique<Sein::Direct3D12::Device>();
        device_->Create(handle, width, height);

        // �R�}���h���X�g�̍쐬
        execute_command_list_ = device_->CreateCommandList(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
        store_command_list_ = device_->CreateCommandList(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
      }

      /**
       *  @brief  ���s����
       */
      void Execute() override
      {
        // ���s�p�R�}���h���X�g�ƍ쐬�p�R�}���h���X�g����������
        execute_command_list_.swap(store_command_list_);

        // �I�u�W�F�N�g�̕`��ɕK�v�Ȃ���
        // ���[���h���W
        // �J����
        // �v���W�F�N�V����
        // ���b�V��
        // ���C�g
        // �{�[���s��

        // �O�t���[���ō쐬�����R�}���h�̃��X�g�����s����(GPU)
        device_->ExecuteCommandLists(execute_command_list_.get());

        // TODO:�O�t���[���̃Q�[����񂩂�R�}���h�̃��X�g���쐬����(�L���[�Ɋi�[����Ă���\��)

        // TODO:BeginScene�AEndScene���̃��\�[�X�w���ύX�ł���悤��
        auto buffer_index = device_->GetNextBackBufferIndex();
        device_->BeginScene(store_command_list_.get(), buffer_index);
        device_->EndScene(store_command_list_.get(), buffer_index);
      }
      
      /**
       *  @brief  ��ʂ��X�V����
       */
      void Present() override
      {
        // TODO:�`��I���҂����s��

        // ��ʂ̍X�V���s��
        device_->Present();
      }
    
      /**
       *  @brief  �I���������s��
       */
      void Destroy() override
      {
        store_command_list_.reset();
        execute_command_list_.reset();
        device_.reset();
      }

    private:
      std::unique_ptr<Sein::Direct3D12::Device> device_;                      ///< �f�o�C�X
      std::shared_ptr<Sein::Direct3D12::ICommandList> execute_command_list_;  ///< ���s�p�R�}���h���X�g
      std::shared_ptr<Sein::Direct3D12::ICommandList> store_command_list_;    ///< �쐬�p�R�}���h���X�g
    };
  };

  /**
   *  @brief  �����_���[���쐬����
   *  @param  handle:�E�B���h�E�n���h��
   *  @param  width:�E�B���h�E�̉���
   *  @param  height:�E�B���h�E�̏c��
   *  @return �����_���[�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IRenderer> IRenderer::Create(HWND handle, std::uint32_t width, std::uint32_t height)
  {
    auto renderer = std::make_unique<Renderer>();

    renderer->Create(handle, width, height);

    return renderer;
  }
};