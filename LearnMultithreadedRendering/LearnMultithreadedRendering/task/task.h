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
#include <vector>

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
     *  @param  delta_time:フレームの経過時間
     */
    virtual void Execute(std::uint64_t delta_time) = 0;

    /**
     *  @brief  依存関係にあるタスクのリストを取得する
     *  @return 依存関係にあるタスクのリスト
     */
    virtual const std::vector<ITask*> GetDependenceTaskList() = 0;

    /**
     *  @brief  タスクが終了したか?
     *  @return 終了フラグ
     */
    virtual bool Finished() const = 0;

    /**
     *  @brief  タスクを作成する
     *  @param  task_function:タスク関数
     *  @param  dependence_task_list:タスクが依存するタスクのリスト
     *  @return タスクインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<ITask> Create(std::function<void(std::uint64_t delta_time)> task_function, const std::vector<ITask*>& dependence_task_list);
  };
};