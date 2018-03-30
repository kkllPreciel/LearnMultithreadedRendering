/**
 *  @file     renderer.cc
 *  @brief    �����_���[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/17
 *  @version  1.0
 */

 // include
#include <thread>
#include <atomic>
#include <mutex>
#include <Sein/Direct3D12/direct3d12_device.h>
#include "renderer/renderer.h"


// TODO:�폜
#include <Sein/Direct3D12/vertex_buffer.h>
#include <Sein/Direct3D12/index_buffer.h>

namespace App
{
  namespace
  {
    // TODO:�폜
    // �A���C�������g��1�o�C�g�ɐݒ�
#pragma pack(push, 1)
    struct Vertex
    {
      DirectX::XMFLOAT3 position; ///< ���W
      DirectX::XMFLOAT3 normal;   ///< �@���x�N�g��
      DirectX::XMFLOAT2 texcoord; ///< �e�N�X�`��UV���W
    };
#pragma pack(pop)
    static Sein::Direct3D12::VertexBuffer vertex_buffer;
    static Sein::Direct3D12::IndexBuffer index_buffer;

    /**
     *  @brief  �����_���[�p�N���X
     */
    class Renderer final : public IRenderer
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Renderer() : device_(nullptr), execute_command_list_(nullptr), store_command_list_(nullptr), thread_(nullptr), terminate_(false), processing_(false)
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

        // �X���b�h�̍쐬
        terminate_ = false;
        processing_ = false;
        thread_ = std::make_unique<std::thread>(&Renderer::ThreadMain, this);


        // TODO�폜
        Vertex vertices[] = {
          { { 0.0f, 0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
          { { 0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
          { { -0.25f, -0.25f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f } },
        };
        std::uint32_t indices[] = { 0, 1, 2 };
        vertex_buffer.Create(const_cast<ID3D12Device*>(&device_->GetDevice()), sizeof(Vertex) * 3, sizeof(Vertex), &vertices);
        index_buffer.Create(const_cast<ID3D12Device*>(&device_->GetDevice()), sizeof(std::uint32_t) * 3, &indices, DXGI_FORMAT_R32_UINT);
      }

      /**
       *  @brief  ���s����
       */
      void Execute() override
      {
        // �����_�����O�X���b�h���N������
        processing_ = true;
        condition_.notify_all();
      }
      
      /**
       *  @brief  ��ʂ��X�V����
       */
      void Present() override
      {
        // �����_�����O�X���b�h�̏����̏I���҂����s��
        // �����ɗ���܂łɏ������I�����Ă���\��������
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [&] {return false == processing_; });

        // TODO:�`��I���҂����s��

        // ��ʂ̍X�V���s��
        device_->Present();
      }
    
      /**
       *  @brief  �I���������s��
       */
      void Destroy() override
      {
        // �����_�����O�X���b�h�̏I���҂����s��
        if (thread_ && thread_->joinable())
        {
          terminate_ = true;
          condition_.notify_all();
          thread_->join();
          thread_.reset();
        }

        store_command_list_.reset();
        execute_command_list_.reset();
        device_.reset();
      }

    private:
      /**
       *  @brief  �X���b�h�̃��C���֐�
       */
      void ThreadMain()
      {
        // �I���t���O�������Ă��Ȃ���Ύ��s
        while (terminate_ == false)
        {
          // �X���b�h�̑ҋ@
          std::unique_lock<std::mutex> lock(mutex_);
          condition_.wait(lock, [&] { return processing_ || terminate_; });

          if (terminate_)
          {
            continue;
          }

          // ���s�p�R�}���h���X�g�ƍ쐬�p�R�}���h���X�g����������
          execute_command_list_.swap(store_command_list_);

          // �O�t���[���ō쐬�����R�}���h�̃��X�g�����s����(GPU)
          device_->ExecuteCommandLists(execute_command_list_.get());

          // TODO:�O�t���[���̃Q�[����񂩂�R�}���h�̃��X�g���쐬����(�L���[�Ɋi�[����Ă���\��)

          // �I�u�W�F�N�g�̕`��ɕK�v�Ȃ���
          // ���[���h���W
          // �J����
          // �v���W�F�N�V����
          // ���b�V��
          // ���C�g
          // �{�[���s��

          // TODO:BeginScene�AEndScene���̃��\�[�X�w���ύX�ł���悤��
          auto buffer_index = device_->GetNextBackBufferIndex();
          device_->BeginScene(store_command_list_.get(), buffer_index);

          // TODO:�폜
          store_command_list_->SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
          store_command_list_->SetVertexBuffers(0, 1, &(vertex_buffer.GetView()));
          store_command_list_->SetIndexBuffer(&(index_buffer.GetView()));
          device_->Render(store_command_list_.get(), 3, 1);

          device_->EndScene(store_command_list_.get(), buffer_index);


          // �X���b�h�̏����I���ʒm
          processing_ = false;
          condition_.notify_all();
        }
      }

    private:
      std::unique_ptr<std::thread> thread_;                                   ///< �X���b�h
      std::atomic<bool> terminate_;                                           ///< �I���t���O
      bool processing_;                                                       ///< �������t���O
      std::mutex mutex_;                                                      ///< �X���b�h�Ԕr�������p
      std::condition_variable condition_;                                     ///< �X���b�h�Ԏ��s�ҋ@�p
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