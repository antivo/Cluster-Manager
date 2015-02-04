#include "console_input_disabler.h"

namespace console {
  InputDisabler::InputDisabler() : hStdin{ GetStdHandle(STD_INPUT_HANDLE) }  {
    GetConsoleMode(hStdin, &mode);
    mode &= ~ENABLE_ECHO_INPUT;
    SetConsoleMode(hStdin, mode);
  }

  InputDisabler::~InputDisabler() {
    mode |= ENABLE_ECHO_INPUT;
    SetConsoleMode(hStdin, mode);
  }
}