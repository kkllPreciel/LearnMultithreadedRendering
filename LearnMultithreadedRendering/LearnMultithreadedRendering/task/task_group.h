/**
 *  @file     task_group.h
 *  @brief    タスクグループに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/04/20
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
#include "task/task.h"

namespace App
{
  /**
   *  @brief  タスクグループ用インターフェイス
   */
  class ITaskGroup
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    ITaskGroup() = default;

    /**
     *  @brief  デストラクタ
     */
    virtual ~ITaskGroup() = default;

    /**
     *  @brief  コピーコンストラクタ
     *  @param  other:コピー元のインスタンス
     */
    ITaskGroup(const ITaskGroup& other) = delete;
  
    /**
     *  @brief  代入演算子オペレータ
     *  @param  other:代入元のインスタンス
     *  @return 代入後のインスタンス
     */
    ITaskGroup& operator = (const ITaskGroup& other) = delete;

    /**
     *  @brief  タスクグループが実行可能か?
     *  @return 実行可能フラグ
     */
    virtual bool Ready() = 0;

    /**
     *  @brief  タスクを取得する
     *  @return タスクへのシェアードポインタ
     */
    virtual std::shared_ptr<ITask> Pop() = 0;

    /**
     *  @brief  タスクグループ内の全タスクが終了したか?
     *  @return 終了フラグ
     */
    virtual bool Finished() = 0;

    /**
     *  @brief  終了待ちを行う
     */
    virtual void Wait() = 0;

    /**
     *  @brief  タスクグループの全タスク終了時のイベントに登録する
     *  @param  function:タスクグループの全タスク終了時に実行する関数
     */
    virtual void RegisterFinishEvent(std::function<void()> function) = 0;

    /**
     *  @brief  タスクグループを作成する
     *  @param  task_list:タスクのリスト
     *  @param  dependence_task_group_list:タスクグループが依存するタスクグループのリスト
     *  @return タスクグループインターフェイスへのシェアードポインタ
     */
    static std::shared_ptr<ITaskGroup> Create(std::vector<std::shared_ptr<ITask>>& task_list, std::vector<std::shared_ptr<ITaskGroup>>& dependence_task_group_list);
  };
};