/**
 *  @file     task.cc
 *  @brief    タスクに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

 // include
#include "task/task.h"
#include <cassert>
#include <atomic>
#include <vector>

namespace App
{
  namespace
  {
    /**
     *  @brief  タスク用クラス
     */
    class Task final : public ITask
    {
    public:
      /**
       *  @brief  コンストラクタ
       *  @param  task:タスク関数
       */
      Task(std::function<void(std::uint64_t)> task) : task_(task), finished_(false), executing_(false), callback_list_({})
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~Task()
      {

      }

      /**
       *  @brief  タスクを実行する
       *  @param  delta_time:フレームの経過時間
       */
      void Execute(std::uint64_t delta_time) override
      {
        assert(!executing_ && !finished_);
        executing_ = true;

        task_(delta_time);
        
        executing_ = false;
        finished_ = true;

        // 終了イベントを通知する
        for (auto callback : callback_list_)
        {
          callback();
        }
      }

      /**
       *  @brief  タスク終了時のイベントに登録する
       *  @param  function:タスク終了時に実行する関数
       */
      void RegisterFinishEvent(std::function<void()> function) override
      {
        callback_list_.emplace_back(function);
      }

      /**
       *  @brief  タスクが終了したか?
       *  @return 終了フラグ
       */
      bool Finished() const override
      {
        return finished_;
      }

    private:
      std::function<void(std::uint64_t)> task_;           ///< タスク関数
      bool finished_;                                     ///< 終了済みフラグ
      bool executing_;                                    ///< 実行中フラグ
      std::vector<std::function<void()>> callback_list_;  ///< タスク終了時に実行する関数のリスト
    };
  };

  /**
   *  @brief  タスクを作成する
   *  @param  task_function:タスク関数
   *  @return タスクインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<ITask> ITask::Create(std::function<void(std::uint64_t delta_time)> task_function)
  {
    auto task = std::make_shared<Task>(task_function);
    return task;
  }
};