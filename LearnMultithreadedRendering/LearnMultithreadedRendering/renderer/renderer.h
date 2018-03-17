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
#include <windef.h>

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
     *  @return レンダラーインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<IRenderer> Create(HWND handle, std::uint32_t width, std::uint32_t height);
  };
};