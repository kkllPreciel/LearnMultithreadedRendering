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
      TaskQueue() : task_list_({}), current_index_(0)
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~TaskQueue()
      {
        task_list_.clear();
      }

      /**
       *  @brief  実行の準備を行う
       */
      void MakeReady() override
      {
        // TODO:タスクの依存関係順にソートする

        current_index_ = 0;
      }

      /**
       *  @brief  タスクを追加する
       *  @param  task:追加するタスク
       */
      void Push(std::shared_ptr<ITask> task) override
      {
        task_list_.emplace_back(task);
      }
  
      /**
       *  @brief  タスクを取得する
       *  @return タスクへのシェアードポインタ
       */
      std::shared_ptr<ITask> Pop() override
      {
        // タスクの依存関係順にソートされている筈なので、先頭から実行していく
        // タスクグループ内のタスクが全て終了したら、次のタスクグループの処理を行う。
        // タスクグループ内のタスクの終了待ちを行わねばならない

        // current_index_はatomic変数なので
        // 1つのタスクが複数のスレッドで一緒に実行される事が起きない
        std::uint32_t index = current_index_++;

        if (task_list_.size() <= index)
        {
          return nullptr;
        }

        return task_list_[index];
      }

      /**
       *  @brief  キューをクリアする
       */
      void Clear() override
      {
        task_list_.clear();
      }

    private:
      std::vector<std::shared_ptr<ITask>> task_list_; ///< タスクのリスト
      std::atomic<std::uint32_t> current_index_;      ///< タスクリストの何番まで実行したかの番号
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