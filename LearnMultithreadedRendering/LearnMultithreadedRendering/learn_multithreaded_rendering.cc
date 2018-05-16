#include "learn_multithreaded_rendering.h"
#include <thread>
#include "model/mesh.h"

static std::shared_ptr<App::IMesh> mesh;
static DirectX::XMFLOAT4X4 matrix;
static float angle = 0.0f;


#include "../MeshLoader/MeshLoader/mesh_loader.h"

/**
 *  @brief  �R���X�g���N�^
 *  @param  parent:�e�E�B�W�F�b�g
 */
LearnMultithreadedRendering::LearnMultithreadedRendering(QWidget *parent)
    : QWidget(parent), renderer_(nullptr), scheduler_(nullptr)
{
    ui.setupUi(this);

    // �X�P�W���[���̍쐬
    scheduler_ = App::ITaskScheduler::Create(std::thread::hardware_concurrency() - 1);

    // �����_���[�̍쐬
    renderer_ = App::IRenderer::Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height(), scheduler_);

    // ���C�����[�v�Ăяo���ݒ�
    timer = std::make_unique<QTimer>();
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(MainLoop()));
    timer->start(1000 / 60);


    // TODO:�폜
    DirectX::XMFLOAT3 positions[3] = {
      { 0.0f, 1.0f, 0.0f},
      { 1.0f, 0.0f, 0.0f },
      { -1.0f, 0.0f, 0.0f },
    };
    mesh = App::IMesh::CreateForTriangle(renderer_.get(), positions);
    DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f));

    auto mesh_loader = MeshLoader::IMeshLoader::Create();
    auto mesh_data = mesh_loader->LoadFromObj("Resources\\Model\\torus.obj");
    mesh = App::IMesh::CreateFromMeshData(renderer_.get(), mesh_data);
}

/**
 *  @brief  �f�X�g���N�^
 */
LearnMultithreadedRendering::~LearnMultithreadedRendering()
{
  if (timer->isActive())
  {
    timer->stop();
  }

  renderer_.reset();
  scheduler_.reset();
}

// 1�t���[���ɂ����镽�ώ��Ԃ��v������
std::uint32_t count(0);
std::uint64_t average(0);
std::uint64_t all(0);

/**
 *  @brief  ���C�����[�v
 */
void LearnMultithreadedRendering::MainLoop()
{
  // �^�X�N�X�P�W���[���̃e�X�g
  auto start = std::chrono::system_clock::now();

  // �񓯊��œ���������(�Q�[�����[�v�A�����_���[�AGPU)
  // �Q�[�����[�v�������_���[�փI�u�W�F�N�g���v�b�V��
  // ���̃t���[���Ń����_���[���R�}���h�o�b�t�@���쐬
  // ����Ɏ��̃t���[����GPU�������_�����O����ʂ��X�V�B

  // TODO:�����_�����O�J�n(�R�}���h�o�b�t�@���쐬���AGPU�ɏ����𓊂���)
  renderer_->Execute();

  // TODO:�Q�[���I�u�W�F�N�g�̍X�V
  scheduler_->Execute(0);
  DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f), DirectX::XMMatrixRotationZ(angle += DirectX::XM_PI / 180.0f)));
  renderer_->Register(mesh->GetVertexBuffer(), mesh->GetIndexBuffer(), mesh->GetIndexCount(), matrix);

  // �����_�����O�I���҂�(GPU�̏����I���҂�)
  renderer_->Present();

  auto delta = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start);
  wchar_t buf[256] = { 0 };
  swprintf_s<256>(buf, L"delta time is %lld\n", delta.count());
  OutputDebugString(buf);

  all += delta.count();
  average = all / ++count;
}