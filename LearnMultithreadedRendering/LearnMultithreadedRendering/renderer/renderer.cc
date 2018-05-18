/**
 *  @file     renderer.cc
 *  @brief    �����_���[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/17
 *  @version  1.0
 */

 // include
#include "renderer/renderer.h"
#include <Sein/Direct3D12/direct3d12_device.h>
#include <Sein/Direct3D12/shader_resource_buffer.h>
#include "renderer/double_command_list.h"
#include "task/task.h" 
#include "task/task_group.h" 

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
     *  @brief  �r���[�p�N���X
     */
    class View
    {
    public:
      std::uint32_t view_id_;
      DirectX::XMFLOAT4X4 view_matrix_;
      DirectX::XMFLOAT4X4 projection_matrix_;
      D3D12_VIEWPORT viewport_;
      D3D12_RECT scissor_;
    };

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
     *  @brief  �����_���[�p�N���X
     */
    class Renderer final : public IRenderer
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Renderer() : device_(nullptr), double_command_list_(nullptr),
        execute_render_object_list_(nullptr), store_render_object_list_(nullptr),
        scheduler_(nullptr), task_group_(nullptr)
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
       *  @param  scheduler:�^�X�N�X�P�W���[��
       */
      void Create(HWND handle, std::uint32_t width, std::uint32_t height, std::shared_ptr<ITaskScheduler> scheduler)
      {
        assert(device_ == nullptr);
        device_ = std::make_unique<Sein::Direct3D12::Device>();
        device_->Create(handle, width, height);

        assert(scheduler_ == nullptr);
        scheduler_ = scheduler;

        // �R�}���h���X�g�̍쐬
        double_command_list_ = IDoubleCommandList::Create(device_.get(), D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);

        // �����_�[�I�u�W�F�N�g�L���[�̍쐬
        execute_render_object_list_ = std::make_unique<std::vector<RenderObject>>();
        store_render_object_list_ = std::make_unique<std::vector<RenderObject>>();

        // �萔�o�b�t�@�̍쐬
        constant_buffer_ = device_->CreateConstantBuffer(sizeof(ConstantBuffer));
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.world_), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.view_), DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&(constant_buffer_instance_.projection_), DirectX::XMMatrixIdentity());

        // �V�F�[�_�[���\�[�X�o�b�t�@(StructuredBuffer)�̍쐬
        resource_buffer_ = device_->CreateShaderResourceBuffer(10000, sizeof(DirectX::XMFLOAT4X4));

        // �r���[�̍쐬
        view_ = std::make_unique<View>();
        view_->view_id_ = 1;
        DirectX::XMStoreFloat4x4(&(view_->view_matrix_), DirectX::XMMatrixRotationZ(DirectX::XM_PI / 2));
        DirectX::XMStoreFloat4x4(&(view_->projection_matrix_), DirectX::XMMatrixIdentity());
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
       *  @brief  �r���[��o�^����
       *  @param  view_id:�r���[��ID
       *  @param  view_matrix:�r���[�s��
       *  @param  projection_matrix:�v���W�F�N�V�����s��
       *  @param  viewport:�r���[�|�[�g��`�̐ݒ�
       *  @param  scissor:�V�U�[��`�̐ݒ�
       */
      void RegisterView(const std::uint32_t view_id, const DirectX::XMFLOAT4X4& view_matrix, const DirectX::XMFLOAT4X4& projection_matrix, const D3D12_VIEWPORT viewport, const D3D12_RECT scissor) override
      {
        assert(!view_);

        // �r���[�̍쐬
        view_->view_id_ = view_id;
        view_->view_matrix_ = view_matrix;
        view_->projection_matrix_ = projection_matrix;
        view_->viewport_ = viewport;
        view_->scissor_ = scissor;
      }

      /**
       *  @brief  ���s����
       */
      void Execute() override
      {
        assert(task_group_ == nullptr);
        assert(scheduler_ != nullptr);

        // �^�X�N�̓o�^
        task_group_ = ITaskGroup::Create(std::vector<std::shared_ptr<ITask>>({ ITask::Create([&](std::uint64_t delta_time) { ThreadMain(); }) }), std::vector<std::shared_ptr<ITaskGroup>>());
        scheduler_->Register(task_group_);
      }
      
      /**
       *  @brief  ��ʂ��X�V����
       */
      void Present() override
      {
        // �^�X�N�̏I���҂�
        // �����ɗ���܂łɏ������I�����Ă���\��������
        task_group_->Wait();
        task_group_.reset();

        // TODO:�`��I���҂����s��

        // ��ʂ̍X�V���s��
        device_->Present();
      }
    
      /**
       *  @brief  �I���������s��
       */
      void Destroy() override
      {
        store_render_object_list_.reset();
        execute_render_object_list_.reset();

        double_command_list_.reset();
        device_.reset();
      }

    private:
      /**
       *  @brief  �X���b�h�̃��C���֐�
       */
      void ThreadMain()
      {
        // ���s�p�R�}���h���X�g�ƍ쐬�p�R�}���h���X�g����������
        double_command_list_->Swap();

        // �O�t���[���ō쐬�����R�}���h�̃��X�g�����s����(GPU)
        device_->ExecuteCommandLists(const_cast<Sein::Direct3D12::ICommandList*>(&(double_command_list_->GetFrontCommandList())));

        decltype(auto) store_command_list = double_command_list_->GetBackCommandList();

        // ���s�p�`��I�u�W�F�N�g�̃L���[�ƍ쐬�p�`��I�u�W�F�N�g�̃L���[����������
        execute_render_object_list_.swap(store_render_object_list_);
        store_render_object_list_->clear();

        // �`�悷��I�u�W�F�N�g�����݂��Ȃ�
        if (execute_render_object_list_->size() <= 0)
        {
          return;
        }

        // TODO:BeginScene�AEndScene���̃��\�[�X�w���ύX�ł���悤��
        auto buffer_index = device_->GetNextBackBufferIndex();
        device_->BeginScene(&store_command_list, buffer_index);

        // TODO:�O�t���[���̃Q�[����񂩂�R�}���h�̃��X�g���쐬����(�L���[�Ɋi�[����Ă���\��)

        // TODO:�h���[�R�[���o�b�`���O�A�_�C�i�~�b�N�o�b�`���O
        // TODO:draw in direct

        // �萔�o�b�t�@�̐ݒ�(�r���[�A�v���W�F�N�V����)
        constant_buffer_instance_.view_ = view_->view_matrix_;
        constant_buffer_instance_.projection_ = view_->projection_matrix_;
        constant_buffer_->Map(sizeof(ConstantBuffer), &(constant_buffer_instance_));

        // �V�F�[�_�[���\�[�X�o�b�t�@�̐ݒ�(���[���h)
        instance_buffer_.clear();
        for (auto& render_object : *execute_render_object_list_)
        {
          instance_buffer_.emplace_back(render_object.matrix_);
        }
        resource_buffer_->Map(instance_buffer_.data(), sizeof(DirectX::XMFLOAT4X4) * 10000);

        auto& vertex_buffer = const_cast<Sein::Direct3D12::IVertexBuffer&>(execute_render_object_list_->at(0).vertex_buffer_);
        auto& index_buffer = const_cast<Sein::Direct3D12::IIndexBuffer&>(execute_render_object_list_->at(0).index_buffer_);
        auto index_count = execute_render_object_list_->at(0).index_count_;
        store_command_list.SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        store_command_list.SetVertexBuffers(0, 1, &(vertex_buffer.GetView()));
        store_command_list.SetIndexBuffer(&(index_buffer.GetView()));
        device_->Render(&store_command_list, index_count, 10000);

        // TODO:�r���[�|�[�g�̐ݒ�
        // TODO:�V�U�[��`�̐ݒ�

        device_->EndScene(&store_command_list, buffer_index);

        // �I�u�W�F�N�g�̕`��ɕK�v�Ȃ���
        // ���[���h���W
        // �J����
        // �v���W�F�N�V����
        // ���b�V��
        // ���C�g
        // �{�[���s��
      }

    private:
      std::unique_ptr<Sein::Direct3D12::Device> device_;                      ///< �f�o�C�X
      std::shared_ptr<App::IDoubleCommandList> double_command_list_;          ///< �R�}���h���X�g(�_�u���o�b�t�@�����O����)

      std::unique_ptr<std::vector<RenderObject>> execute_render_object_list_; ///< ���s�p�`��I�u�W�F�N�g�̃��X�g
      std::unique_ptr<std::vector<RenderObject>> store_render_object_list_;   ///< �쐬�p�`��I�u�W�F�N�g�̃��X�g


      ConstantBuffer constant_buffer_instance_;                               ///< �R���X�^���g�o�b�t�@�̎���
      std::unique_ptr<Sein::Direct3D12::IConstantBuffer> constant_buffer_;    ///< �萔�o�b�t�@
      std::vector<DirectX::XMFLOAT4X4> instance_buffer_;                      ///< �C���X�^���X�o�b�t�@�̎���
      std::unique_ptr<Sein::Direct3D12::ShaderResourceBuffer> resource_buffer_;

      std::unique_ptr<View> view_;                                            ///< �r���[

      std::shared_ptr<ITaskScheduler> scheduler_;                             ///< �X�P�W���[��
      std::shared_ptr<ITaskGroup> task_group_;                                ///< �^�X�N�O���[�v
    };
  };

  /**
   *  @brief  �����_���[���쐬����
   *  @param  handle:�E�B���h�E�n���h��
   *  @param  width:�E�B���h�E�̉���
   *  @param  height:�E�B���h�E�̏c��
   *  @param  scheduler:�^�X�N�X�P�W���[��
   *  @return �����_���[�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
   */
  std::shared_ptr<IRenderer> IRenderer::Create(HWND handle, std::uint32_t width, std::uint32_t height, std::shared_ptr<ITaskScheduler> scheduler)
  {
    auto renderer = std::make_unique<Renderer>();

    renderer->Create(handle, width, height, scheduler);

    return renderer;
  }
};