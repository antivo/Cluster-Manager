#ifndef MEMORY_COMMON
#define MEMORY_COMMON

#include <memory>

namespace memory {
  /// template function that creates unique pointer to object with assigned deleter
  template <typename T, typename Deleter, typename... Args>
  auto make_unique_with_deleter(Deleter deleter, Args&&... args) -> std::unique_ptr<T, Deleter> 
  {
    return std::unique_ptr<T, Deleter>(new T(std::forward<Args>(args)...), deleter);
  }

  /// RAII class to allocate and free space 
  /// intendent to use with primitive types to create storage for compatibility with C functions -> does not support emplacement on purpouse
  template<typename T>
  class DynamicArray {
  public:
    explicit DynamicArray(std::size_t size) : pointerToArray(new T[size]) {}
   
    ~DynamicArray()  {
      delete [] pointerToArray;
    }

    T* get() {
      return pointerToArray;
    }

  private:
    T* pointerToArray;
  };
}

#endif