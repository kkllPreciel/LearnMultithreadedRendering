#include "learn_multithreaded_rendering.h"
#include "model/mesh.h"

static std::shared_ptr<App::IMesh> mesh;
static DirectX::XMFLOAT4X4 matrix;


#include "task/task_scheduler.h"
#include <thread>

static std::shared_ptr<App::ITaskScheduler> scheduler;

/**
 *  @brief  コンストラクタ
 *  @param  parent:親ウィジェット
 */
LearnMultithreadedRendering::LearnMultithreadedRendering(QWidget *parent)
    : QWidget(parent), renderer_(nullptr)
{
    ui.setupUi(this);

    // レンダラーの作成
    renderer_ = App::IRenderer::Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height());

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
    
    // タスクスケジューラのテスト
    scheduler = App::ITaskScheduler::Create(std::thread::hardware_concurrency() - 1);
    auto start = std::chrono::system_clock::now();

    for (auto i = 0; i < 10000; ++i)
    {
      // タスクグループ「A」は依存関係なし
      auto task = App::ITask::Create([]() { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is a\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
      auto a = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>());
      scheduler->Register(a);

      // タスクグループ「B」は「A」に依存する
      task = App::ITask::Create([]() { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is b\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
      auto b = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ a }));
      scheduler->Register(b);

      // タスクグループ「C」は「A」と「B」に依存する
      task = App::ITask::Create([]() { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is c\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
      auto c = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ a, b }));
      scheduler->Register(c);

      // タスクグループ「D」は依存関係なし
      task = App::ITask::Create([]() { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is d\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
      auto d = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>());
      scheduler->Register(d);

      // タスクグループ「E」は「D」に依存する
      task = App::ITask::Create([]() { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is e\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
      auto e = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ d }));
      scheduler->Register(e);

      // タスクグループ「F」は「D」と「E」に依存する
      task = App::ITask::Create([]() { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is f\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
      auto f = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ d, e }));
      scheduler->Register(f);

      // タスクグループ「G」は「C」と「F」に依存する
      task = App::ITask::Create([]() { wchar_t buf[256] = { 0 }; swprintf_s<256>(buf, L"task is g\n"); OutputDebugString(buf); std::this_thread::sleep_for(std::chrono::milliseconds(10)); });
      auto g = App::ITaskGroup::Create(std::vector<std::shared_ptr<App::ITask>>({ task }), std::vector<std::shared_ptr<App::ITaskGroup>>({ c, f }));
      scheduler->Register(g);

      // 上記状態のタスクグループをタスクスケジューラに登録すると
      // 下記順番で実行される筈(括弧内は順番が変わるかも)
      // (A,D)、(B,E)、(C,F)、G
    }

    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    wchar_t buf[256] = { 0 };
    swprintf_s<256>(buf, L"delta time is %lld\n", delta.count());
    OutputDebugString(buf);
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
}

/**
 *  @brief  メインループ
 */
void LearnMultithreadedRendering::MainLoop()
{
  // 非同期で動かすもの(ゲームループ、レンダラー、GPU)
  // ゲームループがレンダラーへオブジェクトをプッシュ
  // 次のフレームでレンダラーがコマンドバッファを作成
  // さらに次のフレームでGPUがレンダリングし画面を更新。

  // TODO:レンダリング開始(コマンドバッファを作成し、GPUに処理を投げる)
  renderer_->Execute();

  // TODO:ゲームオブジェクトの更新
  renderer_->Register(mesh->GetVertexBuffer(), mesh->GetIndexBuffer(), mesh->GetIndexCount(), matrix);

  // レンダリング終了待ち(GPUの処理終了待ち)
  renderer_->Present();
}