#ifndef CONSOLE_COLOR_MANIPULATOR
#define CONSOLE_COLOR_MANIPULATOR

#include <windows.h>

#include <memory>

namespace console {
  class ColorManipulator {
  public:  
    explicit ColorManipulator(const WORD color);
    ~ColorManipulator();

  private:
    HANDLE hstdout;
    std::unique_ptr<CONSOLE_SCREEN_BUFFER_INFO> csbi;

    ColorManipulator(const ColorManipulator&) = delete;
    ColorManipulator& operator=(const ColorManipulator) = delete;
  };
}

#endif