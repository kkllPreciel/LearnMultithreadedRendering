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
      TaskThread() : thread_(nullptr), queue_(), terminate_(false), executing_(false)
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
       */
      void Execute() override
      {
        executing_ = true;

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
          executing_ = true;

          // スレッドを起床する
          condition_.notify_one();

          // スレッドの終了待ちを行う
          thread_->join();

          thread_.reset();
        }
      }

      /**
       *  @brief  実行中か?
       *  @return 実行中フラグ
       */
      bool Executing() const override
      {
        return executing_;
      }

    private:
      /**
       *  @brief  タスクスレッドのメイン関数
       */
      void Run()
      {
        {
          // 排他処理
          std::unique_lock<std::mutex> lk(mutex_);

          // スレッドの待機
          condition_.wait(lk, [&]{ return executing_; });
        }
        
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
            task->Execute();
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

            executing_ = false;

            // スレッドの待機
            condition_.wait(lk, [&] { return executing_; });
          }
        }

        executing_ = false;
      }

    private:
      std::unique_ptr<std::thread> thread_; ///< スレッド
      std::shared_ptr<ITaskQueue> queue_;   ///< タスクキュー
      std::mutex mutex_;                    ///< ミューテックス
      std::condition_variable condition_;   ///< 条件変数
      bool terminate_;                      ///< 終了フラグ
      bool executing_;                      ///< 実行中フラグ
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