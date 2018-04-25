/**
 *  @file     task_group.cc
 *  @brief    タスクグループに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/20
 *  @version  1.0
 */

 // include
#include "task/task_group.h"
#include <atomic>
#include <mutex>

namespace App
{
  namespace
  {
    /**
     *  @brief  タスクグループ用クラス
     */
    class TaskGroup final : public ITaskGroup
    {
    public:
      /**
       *  @brief  コンストラクタ
       *  @param  task_list:タスクグループに属するタスクのリスト
       *  @param  dependence_task_group_list:依存先のタスクグループのリスト
       */
      TaskGroup(std::vector<std::shared_ptr<ITask>>& task_list, std::vector<std::shared_ptr<ITaskGroup>>& dependence_task_group_list)
        : task_list_(task_list), dependence_task_group_list_(dependence_task_group_list), executed_index_(0), finished_count_(0), finished_(false), callback_list_({})
      {
        // タスクの終了イベントに登録する
        for (auto task : task_list_)
        {
          task->RegisterFinishEvent([&](){
            if (++finished_count_ == task_list_.size())
            {
              std::unique_lock<std::mutex> lock(mutex_);
              finished_ = true;

              // 終了イベントを通知する
              for (auto callback : callback_list_)
              {
                callback();
              }

              condition_.notify_all();
            }
          });
        }
      }

      /**
       *  @brief  デストラクタ
       */
      ~TaskGroup() override
      {

      }

      /**
       *  @brief  タスクグループが実行可能か?
       *  @return 実行可能フラグ
       */
      bool Ready() override
      {
        for (auto dependence_task_group : dependence_task_group_list_)
        {
          if (dependence_task_group->Finished() == false)
          {
            return false;
          }
        }

        return true;
      }

      /**
       *  @brief  タスクを取得する
       *  @return タスクへのシェアードポインタ
       */
      std::shared_ptr<ITask> Pop() override
      {
        // 依存先のタスクグループが終了していなければ
        // 依存先のタスクグループのタスクを戻す
        for (auto dependence_task_group : dependence_task_group_list_)
        {
          if (dependence_task_group->Finished() == false)
          {
            auto dependence_task = dependence_task_group->Pop();
            if (dependence_task != nullptr)
            {
              return dependence_task;
            }

            // 依存先のタスクグループのタスクが取得できない場合は
            // 依存先のタスクグループの終了待ちを行う
            dependence_task_group->Wait();
          }
        }

        // executed_index_はatomic変数なので
        // 1つのタスクが複数のスレッドで一緒に実行される事が起きない
        std::uint32_t index = executed_index_++;
        if (task_list_.size() <= index)
        {
          return nullptr;
        }

        return task_list_[index];
      }

      /**
       *  @brief  終了待ちを行う
       */
      void Wait() override
      {
        std::unique_lock<std::mutex> lock(mutex_);
        if (finished_ == false)
        {
          condition_.wait(lock, [&]{ return finished_; });
        }
      }

      /**
       *  @brief  タスクグループ内の全タスクが終了したか?
       *  @return 終了フラグ
       */
      bool Finished() override
      {
        return finished_;
      }

      /**
       *  @brief  タスクグループの全タスク終了時のイベントに登録する
       *  @param  function:タスクグループの全タスク終了時に実行する関数
       */
      void RegisterFinishEvent(std::function<void()> function) override
      {
        callback_list_.emplace_back(function);
      }

    private:
      std::vector<std::shared_ptr<ITask>> task_list_;                         ///< タスクグループに属するタスクのリスト
      std::atomic<std::uint32_t> executed_index_;                             ///< タスクリストの何番まで実行したかの番号
      std::atomic<std::uint32_t> finished_count_;                             ///< 実行が終了したタスクの個数
      bool finished_;                                                         ///< 全処理終了済みフラグ
      std::vector<std::shared_ptr<ITaskGroup>>  dependence_task_group_list_;  ///< 依存先のタスクグループのリスト
      std::mutex mutex_;                                                      ///< 排他制御用ミューテックス
      std::condition_variable condition_;                                     ///< 実行待機用条件変数
      std::vector<std::function<void()>> callback_list_;                      ///< タスクグループ内の全タスク終了時に実行する関数のリスト
    };
  };

  /**
   *  @brief  タスクグループを作成する
   *  @param  task_list:タスクのリスト
   *  @param  dependence_task_group_list:タスクグループが依存するタスクグループのリスト
   *  @return タスクグループインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<ITaskGroup> ITaskGroup::Create(std::vector<std::shared_ptr<ITask>>& task_list, std::vector<std::shared_ptr<ITaskGroup>>& dependence_task_group_list)
  {
    auto task_group = std::make_shared<TaskGroup>(task_list, dependence_task_group_list);
    return task_group;
  }
};