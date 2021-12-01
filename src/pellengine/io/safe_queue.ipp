#include "safe_queue.h"

namespace pellengine {

template<typename T>
SafeQueue<T>::SafeQueue() {}

template<typename T>
SafeQueue<T>::~SafeQueue() {}

template<typename T>
unsigned long SafeQueue<T>::size() {
  std::lock_guard<std::mutex> lock(mutex);
  return queue.size();
}

template<typename T>
std::optional<T> SafeQueue<T>::pop() {
  std::lock_guard<std::mutex> lock(mutex);
  if(queue.empty()) {
    return {};
  }

  T tmp = queue.front();
  queue.pop();
  return tmp;
}

template<typename T>
void SafeQueue<T>::push(const T& item) {
  std::lock_guard<std::mutex> lock(mutex);
  queue.push(item);
}

}