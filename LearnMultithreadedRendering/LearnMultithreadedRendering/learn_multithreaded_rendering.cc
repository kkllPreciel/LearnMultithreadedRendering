#include "learn_multithreaded_rendering.h"
#include "model/mesh.h"

static std::shared_ptr<App::IMesh> mesh;
static DirectX::XMFLOAT4X4 matrix;

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
    DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());
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