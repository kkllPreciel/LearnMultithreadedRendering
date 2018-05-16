#include "learn_multithreaded_rendering.h"
#include <thread>
#include "model/mesh.h"

static std::shared_ptr<App::IMesh> mesh;
static DirectX::XMFLOAT4X4 matrix;
static float angle = 0.0f;


#include "../MeshLoader/MeshLoader/mesh_loader.h"

/**
 *  @brief  コンストラクタ
 *  @param  parent:親ウィジェット
 */
LearnMultithreadedRendering::LearnMultithreadedRendering(QWidget *parent)
    : QWidget(parent), renderer_(nullptr), scheduler_(nullptr)
{
    ui.setupUi(this);

    // スケジューラの作成
    scheduler_ = App::ITaskScheduler::Create(std::thread::hardware_concurrency() - 1);

    // レンダラーの作成
    renderer_ = App::IRenderer::Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height(), scheduler_);

    // メインループ呼び出し設定
    timer = std::make_unique<QTimer>();
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(MainLoop()));
    timer->start(1000 / 60);


    // TODO:削除
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
 *  @brief  デストラクタ
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

// 1フレームにかかる平均時間を計測する
std::uint32_t count(0);
std::uint64_t average(0);
std::uint64_t all(0);

/**
 *  @brief  メインループ
 */
void LearnMultithreadedRendering::MainLoop()
{
  // タスクスケジューラのテスト
  auto start = std::chrono::system_clock::now();

  // 非同期で動かすもの(ゲームループ、レンダラー、GPU)
  // ゲームループがレンダラーへオブジェクトをプッシュ
  // 次のフレームでレンダラーがコマンドバッファを作成
  // さらに次のフレームでGPUがレンダリングし画面を更新。

  // TODO:レンダリング開始(コマンドバッファを作成し、GPUに処理を投げる)
  renderer_->Execute();

  // TODO:ゲームオブジェクトの更新
  scheduler_->Execute(0);
  DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f), DirectX::XMMatrixRotationZ(angle += DirectX::XM_PI / 180.0f)));
  renderer_->Register(mesh->GetVertexBuffer(), mesh->GetIndexBuffer(), mesh->GetIndexCount(), matrix);

  // レンダリング終了待ち(GPUの処理終了待ち)
  renderer_->Present();

  auto delta = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start);
  wchar_t buf[256] = { 0 };
  swprintf_s<256>(buf, L"delta time is %lld\n", delta.count());
  OutputDebugString(buf);

  all += delta.count();
  average = all / ++count;
}