/**
 *  @file     renderer.h
 *  @brief    レンダラーに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/17
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <DirectXMath.h>
#include "model/mesh.h"
#include "task/task_scheduler.h"

namespace App
{
  /**
   *  @brief  レンダラー用インターフェイス
   */
  class IRenderer
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    IRenderer() = default;
  
    /**
     *  @brief  デストラクタ
     */
    virtual ~IRenderer() = default;

    /**
     *  @brief  メッシュを作成する
     *  @param  mesh_data:メッシュデータ
     */
    virtual std::shared_ptr<IMesh> CreateMesh(std::shared_ptr<MeshLoader::IMeshData>& mesh_data) = 0;

    /**
     *  @brief  描画オブジェクトの登録を行う
     *  @param  mesh:描画オブジェクトが使用するメッシュ
     *  @param  matrix:描画オブジェクトのワールド空間行列
     */
    virtual void Register(const std::shared_ptr<IMesh>& mesh, const DirectX::XMFLOAT4X4& matrix) = 0;

    /**
     *  @brief  ビューを登録する
     *  @param  view_id:ビューのID
     *  @param  view_matrix:ビュー行列
     *  @param  projection_matrix:プロジェクション行列
     *  @param  viewport:ビューポート矩形の設定
     *  @param  scissor:シザー矩形の設定
     */
    virtual void RegisterView(const std::uint32_t view_id, const DirectX::XMFLOAT4X4& view_matrix, const DirectX::XMFLOAT4X4& projection_matrix, const D3D12_VIEWPORT viewport, const D3D12_RECT scissor) = 0;

    /**
     *  @brief  実行する
     */
    virtual void Execute() = 0;

    /**
     *  @brief  画面を更新する
     */
    virtual void Present() = 0;
  
    /**
     *  @brief  終了処理を行う
     */
    virtual void Destroy() = 0;
  
    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    IRenderer(const IRenderer& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    IRenderer& operator = (const IRenderer& other) = delete;

    /**
     *  @brief  レンダラーを作成する
     *  @param  handle:ウィンドウハンドル
     *  @param  width:ウィンドウの横幅
     *  @param  height:ウィンドウの縦幅
     *  @param  scheduler:タスクスケジューラ
     *  @return レンダラーインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IRenderer> Create(HWND handle, std::uint32_t width, std::uint32_t height, std::shared_ptr<ITaskScheduler> scheduler);
  };
};