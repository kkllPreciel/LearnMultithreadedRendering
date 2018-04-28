/**
 *  @file     task.h
 *  @brief    タスクに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <functional>

namespace App
{
  /**
   *  @brief  タスク用インターフェイス
   */
  class ITask
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    ITask() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~ITask() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    ITask(const ITask& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    ITask& operator = (const ITask& other) = delete;

    /**
     *  @brief  タスクを実行する
     */
    virtual void Execute() = 0;

    /**
     *  @brief  タスクが終了したか?
     *  @return 終了フラグ
     */
    virtual bool Finished() const = 0;

    /**
     *  @brief  タスク終了時のイベントに登録する
     *  @param  function:タスク終了時に実行する関数
     */
    virtual void RegisterFinishEvent(std::function<void()> function) = 0;

    /**
     *  @brief  タスクを作成する
     *  @param  task_function:タスク関数
     *  @return タスクインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<ITask> Create(std::function<void()> task_function);
  };
};