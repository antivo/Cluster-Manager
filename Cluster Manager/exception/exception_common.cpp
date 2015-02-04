#include "exception_common.h"

#include <windows.h>

#include <memory>
#include <array>

#include "memory_common.h"

namespace exception {

   std::shared_ptr<const std::string> formatMessage(const int errorCode) {
    LPTSTR readableMessage = nullptr;  
    size_t size = FormatMessage( 
       FORMAT_MESSAGE_FROM_SYSTEM       // use system message tables to retrieve error text
       |FORMAT_MESSAGE_ALLOCATE_BUFFER  // allocate buffer on local heap for error text
       |FORMAT_MESSAGE_IGNORE_INSERTS,  // we're not (and CANNOT) pass insertion parameters
       nullptr,         // 0 for internal message table
       errorCode, // present the error code
       0,         // auto-determine language to use
       reinterpret_cast<LPTSTR>(&readableMessage), // http://msdn.microsoft.com/en-us/library/ms679351(VS.85).aspx
       0,         // min size for buffer
       nullptr );       // 0 for getting message from system tables
    std::shared_ptr<const std::string> message(nullptr);
    if(size) {
      size_t convertedStringSize;
      auto status = wcstombs_s(&convertedStringSize, nullptr, 0, readableMessage, _TRUNCATE);
      if (0 == status) {
        //char* convertedString = new char[convertedStringSize];
        //std::array<char, 256> convertedString;
        auto convertedString = memory::DynamicArray<char>(convertedStringSize);
        status = wcstombs_s(nullptr, convertedString.get(), convertedStringSize, readableMessage, size);
        if(0 == status) {
          message = std::make_shared<const std::string>(convertedString.get());
        }
      }
    } 
    LocalFree(readableMessage) ; // LocalAlloc to free FORMAT_MESSAGE_ALLOCATE_BUFFER,
    return message;
  }

  Exception::Exception(const int errorCode) : errorCode{errorCode},  message{formatMessage(errorCode)} {}

  Exception::~Exception() {}

  const int Exception::getErrorCode() const {
    return this->errorCode;
  }

  const char* Exception::what() const {
    if(nullptr != message) { 
      return message->c_str();
    } else { 
      return "Unable to clarify source of networking problem.\n";
    } 
  }

}