#ifndef _PELLENGINE_IO_SAFE_QUEUE_H_
#define _PELLENGINE_IO_SAFE_QUEUE_H_

#include <queue>
#include <mutex>
#include <optional>

namespace pellengine {

template<typename T>
class SafeQueue {
 public:
  SafeQueue();
  virtual ~SafeQueue();

  SafeQueue(const SafeQueue&) = delete;
  SafeQueue& operator=(const SafeQueue&) = delete;

  unsigned long size();
  std::optional<T> pop();
  void push(const T& item);

 private:
  std::queue<T> queue;
  std::mutex mutex;
};

}

#include "safe_queue.ipp"
#endif