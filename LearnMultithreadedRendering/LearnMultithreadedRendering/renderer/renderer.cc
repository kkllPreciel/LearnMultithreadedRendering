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

namespace App
{
  namespace
  {
    /**
     *  @brief  �`��I�u�W�F�N�g�p�N���X
     */
    class RenderObject
    {
    public:
      RenderObject(const Sein::Direct3D12::IVertexBuffer& vertex_buffer, const Sein::Direct3D12::IIndexBuffer& index_buffer, const std::uint32_t index_count, const DirectX::XMFLOAT4X4& matrix)
        : vertex_buffer_(vertex_buffer), index_buffer_(index_buffer), index_count_(index_count), matrix_(matrix)
      {

      }

      const Sein::Direct3D12::IVertexBuffer& vertex_buffer_;
      const Sein::Direct3D12::IIndexBuffer& index_buffer_;
      const std::uint32_t index_count_;
      const DirectX::XMFLOAT4X4& matrix_;
    };

    /**
     *  @brief  �����_���[�p�N���X
     */
    class Renderer final : public IRenderer
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Renderer() : device_(nullptr), execute_command_list_(nullptr), store_command_list_(nullptr),
        thread_(nullptr), terminate_(false), processing_(false),
        execute_render_object_list_(nullptr), store_render_object_list_(nullptr)
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

        // �����_�[�I�u�W�F�N�g�L���[�̍쐬
        execute_render_object_list_ = std::make_unique<std::vector<RenderObject>>();
        store_render_object_list_ = std::make_unique<std::vector<RenderObject>>();

        // �X���b�h�̍쐬
        terminate_ = false;
        processing_ = false;
        thread_ = std::make_unique<std::thread>(&Renderer::ThreadMain, this);
      }

      /**
       *  @brief  ���_�o�b�t�@���쐬����
       *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
       *  @return ���_�o�b�t�@�ւ̃��j�[�N�|�C���^
       */
      std::unique_ptr<Sein::Direct3D12::IVertexBuffer> CreateVertexBuffer(const std::uint32_t size_in_bytes) override
      {
        return device_->CreateVertexBuffer(size_in_bytes);
      }
      
      /**
       *  @brief  ���_�C���f�b�N�X�o�b�t�@���쐬����
       *  @param  size_in_bytes:���_�C���f�b�N�X�o�b�t�@�̃T�C�Y(���_�C���f�b�N�X�T�C�Y * ���_�C���f�b�N�X��)
       *  @return ���_�C���f�b�N�X�o�b�t�@�̃��j�[�NID
       */
      std::unique_ptr<Sein::Direct3D12::IIndexBuffer> CreateIndexBuffer(const std::uint32_t size_in_bytes) override
      {
        return device_->CreateIndexBuffer(size_in_bytes);
      }

      /**
       *  @brief  �`��I�u�W�F�N�g�̓o�^���s��
       *  @param  vertex_buffer:�`��I�u�W�F�N�g���g�p���钸�_�o�b�t�@
       *  @param  index_buffer:�`��I�u�W�F�N�g���g�p���钸�_�C���f�b�N�X�o�b�t�@
       *  @param  index_count:���_�C���f�b�N�X�̌�
       *  @param  matrix:�`��I�u�W�F�N�g�̃��[���h��ԍs��
       */
      void Register(const Sein::Direct3D12::IVertexBuffer& vertex_buffer, const Sein::Direct3D12::IIndexBuffer& index_buffer, const std::uint32_t index_count, const DirectX::XMFLOAT4X4& matrix) override
      {
        // TODO:swap�̃^�C�~���O�Ŏ��s����Ă����Ȃ��悤��
        store_render_object_list_->emplace_back(RenderObject(vertex_buffer, index_buffer, index_count, matrix));
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

        store_render_object_list_.reset();
        execute_render_object_list_.reset();

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

          // ���s�p�`��I�u�W�F�N�g�̃L���[�ƍ쐬�p�`��I�u�W�F�N�g�̃L���[����������
          execute_render_object_list_.swap(store_render_object_list_);

          // TODO:BeginScene�AEndScene���̃��\�[�X�w���ύX�ł���悤��
          auto buffer_index = device_->GetNextBackBufferIndex();
          device_->BeginScene(store_command_list_.get(), buffer_index);

          // TODO:�O�t���[���̃Q�[����񂩂�R�}���h�̃��X�g���쐬����(�L���[�Ɋi�[����Ă���\��)

          // TODO:�h���[�R�[���o�b�`���O�A�_�C�i�~�b�N�o�b�`���O
          // TODO:draw in direct

          // �h���[�R�[��
          for (auto& render_object : *execute_render_object_list_)
          {
            auto& vertex_buffer = const_cast<Sein::Direct3D12::IVertexBuffer&>(render_object.vertex_buffer_);
            auto& index_buffer = const_cast<Sein::Direct3D12::IIndexBuffer&>(render_object.index_buffer_);
            auto index_count = render_object.index_count_;
            store_command_list_->SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            store_command_list_->SetVertexBuffers(0, 1, &(vertex_buffer.GetView()));
            store_command_list_->SetIndexBuffer(&(index_buffer.GetView()));
            device_->Render(store_command_list_.get(), index_count, 1);
          }

          device_->EndScene(store_command_list_.get(), buffer_index);

          // �I�u�W�F�N�g�̕`��ɕK�v�Ȃ���
          // ���[���h���W
          // �J����
          // �v���W�F�N�V����
          // ���b�V��
          // ���C�g
          // �{�[���s��

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
      std::unique_ptr<std::vector<RenderObject>> execute_render_object_list_; ///< ���s�p�`��I�u�W�F�N�g�̃��X�g
      std::unique_ptr<std::vector<RenderObject>> store_render_object_list_;   ///< �쐬�p�`��I�u�W�F�N�g�̃��X�g
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