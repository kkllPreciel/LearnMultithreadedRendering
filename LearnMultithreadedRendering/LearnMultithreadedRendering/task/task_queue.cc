/**
 *  @file     task_queue.cc
 *  @brief    タスクキューに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/18
 *  @version  1.0
 */

 // include
#include "task/task_queue.h"
#include <vector>
#include <atomic>
#include <mutex>

namespace App
{
  namespace
  {
    /**
     *  @brief  タスキュー用クラス
     */
    class TaskQueue final : public ITaskQueue
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      TaskQueue() : task_group_list_({}), executed_index_(0)
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~TaskQueue()
      {
        task_group_list_.clear();
      }

      /**
       *  @brief  タスクグループを追加する
       *  @param  task_group:追加するタスクグループ
       */
      void Push(std::shared_ptr<ITaskGroup> task_group) override
      {
        // タスクグループの終了イベントに登録する
        task_group->RegisterFinishEvent([&]() {
          ++executed_index_;
          condition_.notify_all();
        });
        task_group_list_.emplace_back(task_group);
      }
  
      /**
       *  @brief  タスクを取得する
       *  @return タスクへのシェアードポインタ
       */
      std::shared_ptr<ITask> Pop() override
      {
        // 実行中に値が変わってしまう場合があるのでローカル変数にコピーする
        std::uint32_t executed_index = executed_index_;

        // 全タスクグループの全タスクが終了した
        if (task_group_list_.size() <= executed_index)
        {
          return nullptr;
        }

        // 実行可能なタスクグループからタスクを取得する
        for (auto i = 0; i < task_group_list_.size(); ++i)
        {
          // タスクが終了済み
          if (task_group_list_[i]->Finished())
          {
            continue;
          }

          // 実行可能状態ではない(依存先のタスクグループが終了していない)
          if (false == task_group_list_[i]->Ready())
          {
            continue;
          }

          auto task = task_group_list_[i]->Pop();
          if (task != nullptr)
          {
            return task;
          }
        }

        // 実行可能なタスクが存在しないので
        // 実行中のタスクグループが終了するまで待機を行う
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [&] { return executed_index != executed_index_; });

        return nullptr;
      }

    private:
      std::vector<std::shared_ptr<ITaskGroup>>  task_group_list_; ///< タスクグループのリスト
      std::atomic<std::uint32_t> executed_index_;                 ///< タスクグループリストの何番まで実行したかの番号
      std::mutex mutex_;                                          ///< 排他制御用ミューテックス
      std::condition_variable condition_;                         ///< 実行待機用条件変数
    };
  };

  /**
   *  @brief  タスクキューを作成する
   *  @return タスクキューインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<ITaskQueue> ITaskQueue::Create()
  {
    auto queue = std::make_shared<TaskQueue>();
    return queue;
  }
};