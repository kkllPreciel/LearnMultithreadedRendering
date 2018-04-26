/**
 *  @file     task_thread.cc
 *  @brief    タスクスレッド(ワーカースレッド)に関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/04/17
 *  @version  1.0
 */

 // include
#include "task/task_thread.h"
#include <thread>
#include <mutex>

namespace App
{
  namespace
  {
    /**
     *  @brief  タスクスレッド用クラス
     */
    class TaskThread final : public ITaskThread
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      TaskThread() : thread_(nullptr), queue_(), terminate_(false)
      {

      }

      /**
       *  @brief  デストラクタ
       */
      ~TaskThread() override
      {
        Destroy();
      }

      /**
       *  @brief  作成する
       *  @param  queue:タスクキュー
       */
      void Create(std::shared_ptr<ITaskQueue> queue)
      {
        queue_ = queue;
        thread_ = std::make_unique<std::thread>(&TaskThread::Run, this);
      }

      /**
       *  @brief  実行する
       *  @param  delta_time:フレームの経過時間
       */
      void Execute(std::uint64_t delta_time) override
      {
        delta_time_ = delta_time;

        // スレッドを起床する
        condition_.notify_one();
      }
  
      /**
       *  @brief  終了する
       */
      void Destroy() override
      {
        if (thread_ && thread_->joinable())
        {
          // 終了フラグを立てる
          terminate_ = true;

          // スレッドを起床する
          condition_.notify_one();

          // スレッドの終了待ちを行う
          thread_->join();

          thread_.reset();
        }
      }

    private:
      /**
       *  @brief  タスクスレッドのメイン関数
       */
      void Run()
      {
        while (true)
        {
          // 終了フラグが立っていたら終了する
          if (terminate_)
          {
            break;
          }

          std::shared_ptr<ITask> task = queue_->Pop();
          if (task != nullptr)
          {
            // タスクを実行する
            task->Execute(delta_time_);
          }
          else if (false == queue_->Finished())
          {
            continue;
          }
          else
          {
            // タスクが存在しないので待機する

            // 排他処理
            std::unique_lock<std::mutex> lk(mutex_);

            // スレッドの待機
            condition_.wait(lk);
          }
        }
      }

    private:
      std::unique_ptr<std::thread> thread_; ///< スレッド
      std::shared_ptr<ITaskQueue> queue_;   ///< タスクキュー
      std::mutex mutex_;                    ///< ミューテックス
      std::condition_variable condition_;   ///< 条件変数
      std::uint64_t delta_time_;            ///< フレームの経過時間
      bool terminate_;                      ///< 終了フラグ
    };
  };

  /**
   *  @brief  タスクスレッドを作成する
   *  @param  queue:タスクのキュー
   *  @return タスクスレッドインターフェイスへのシェアードポインタ
   */
  std::shared_ptr<ITaskThread> ITaskThread::Create(std::shared_ptr<ITaskQueue> queue)
  {
    auto task_thread = std::make_shared<TaskThread>();
    
    task_thread->Create(queue);

    return task_thread;
  }
};