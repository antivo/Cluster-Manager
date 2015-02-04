#ifndef CONSOLE_INPUT_DISABLER
#define CONSOLE_INPUT_DISABLER

#include <windows.h>

namespace console {
  class InputDisabler{
  public:
    InputDisabler(); 
    ~InputDisabler();

  private:
    HANDLE hStdin;
    DWORD mode;

    InputDisabler(const InputDisabler&) = delete;
    InputDisabler& operator=(const InputDisabler&) = delete;
  };
}

#endif