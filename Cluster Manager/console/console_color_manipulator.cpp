#include "console_color_manipulator.h"

#include <memory>

namespace console {
  ColorManipulator::ColorManipulator(const WORD color) : hstdout{ GetStdHandle(STD_OUTPUT_HANDLE) }, csbi{ std::make_unique<CONSOLE_SCREEN_BUFFER_INFO>() }  {
    GetConsoleScreenBufferInfo( hstdout, csbi.get());
    SetConsoleTextAttribute(hstdout, color);
  }

  ColorManipulator::~ColorManipulator() {
    SetConsoleTextAttribute(hstdout, csbi->wAttributes);
  }
}