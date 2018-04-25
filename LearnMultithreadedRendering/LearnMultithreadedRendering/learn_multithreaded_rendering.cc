#include "learn_multithreaded_rendering.h"
#include "model/mesh.h"

static std::shared_ptr<App::IMesh> mesh;
static DirectX::XMFLOAT4X4 matrix;


#include "task/task_scheduler.h"
#include <thread>

/**
 *  @brief  �R���X�g���N�^
 *  @param  parent:�e�E�B�W�F�b�g
 */
LearnMultithreadedRendering::LearnMultithreadedRendering(QWidget *parent)
    : QWidget(parent), renderer_(nullptr)
{
    ui.setupUi(this);

    // �����_���[�̍쐬
    renderer_ = App::IRenderer::Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height());

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

    // �^�X�N�X�P�W���[���̃e�X�g
    auto scheduler = App::ITaskScheduler::Create(std::thread::hardware_concurrency() - 1);

    // �^�X�N�O���[�v�uA�v�͈ˑ��֌W�Ȃ�
    auto task = App::ITask::Create([](std::uint64_t delta_time) { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is a\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    auto a = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>());
    scheduler->Register(a);

    // �^�X�N�O���[�v�uB�v�́uA�v�Ɉˑ�����
    task = App::ITask::Create([](std::uint64_t delta_time) { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is b\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    auto b = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ a }));
    scheduler->Register(b);

    // �^�X�N�O���[�v�uC�v�́uA�v�ƁuB�v�Ɉˑ�����
    task = App::ITask::Create([](std::uint64_t delta_time) { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is c\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    auto c = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ a, b }));
    scheduler->Register(c);

    // �^�X�N�O���[�v�uD�v�͈ˑ��֌W�Ȃ�
    task = App::ITask::Create([](std::uint64_t delta_time) { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is d\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    auto d = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>());
    scheduler->Register(d);

    // �^�X�N�O���[�v�uE�v�́uD�v�Ɉˑ�����
    task = App::ITask::Create([](std::uint64_t delta_time) { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is e\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    auto e = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ d }));
    scheduler->Register(e);

    // �^�X�N�O���[�v�uF�v�́uD�v�ƁuE�v�Ɉˑ�����
    task = App::ITask::Create([](std::uint64_t delta_time) { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is f\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    auto f = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ d, e }));
    scheduler->Register(f);

    // �^�X�N�O���[�v�uG�v�́uC�v�ƁuF�v�Ɉˑ�����
    task = App::ITask::Create([](std::uint64_t delta_time) { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is g\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    auto g = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ c, f }));
    scheduler->Register(g);

    // ��L��Ԃ̃^�X�N�O���[�v���^�X�N�X�P�W���[���ɓo�^�����
    // ���L���ԂŎ��s����锤(���ʓ��͏��Ԃ��ς�邩��)
    // (A,D)�A(B,E)�A(C,F)�AG
    scheduler->Execute(0);

    /*auto task = App::ITask::Create([](std::uint64_t delta_time) { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });
    auto task_group = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>());
    auto second_task = App::ITask::Create([](std::uint64_t delta_time) {});
    auto second_task_group = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ second_task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ task_group }));
    scheduler->Register(task_group);
    scheduler->Register(second_task_group);
    scheduler->Execute(0);*/
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
}

/**
 *  @brief  ���C�����[�v
 */
void LearnMultithreadedRendering::MainLoop()
{
  // �񓯊��œ���������(�Q�[�����[�v�A�����_���[�AGPU)
  // �Q�[�����[�v�������_���[�փI�u�W�F�N�g���v�b�V��
  // ���̃t���[���Ń����_���[���R�}���h�o�b�t�@���쐬
  // ����Ɏ��̃t���[����GPU�������_�����O����ʂ��X�V�B

  // TODO:�����_�����O�J�n(�R�}���h�o�b�t�@���쐬���AGPU�ɏ����𓊂���)
  renderer_->Execute();

  // TODO:�Q�[���I�u�W�F�N�g�̍X�V
  renderer_->Register(mesh->GetVertexBuffer(), mesh->GetIndexBuffer(), mesh->GetIndexCount(), matrix);

  // �����_�����O�I���҂�(GPU�̏����I���҂�)
  renderer_->Present();
}