#ifndef EXCEPTION_COMMON
#define EXCEPTION_COMMON

#include <string>
#include <memory>

namespace exception {

  /// Formats message for given error
  std::shared_ptr<const std::string> formatMessage(const int errorCode);

  /// Containst error code. Contains readable message of error.
  class Exception : public std::exception {
  public:
    explicit Exception(const int errorCode);
    virtual ~Exception();

    /// Provide error code
    const int getErrorCode() const;
    
    /// Provides a readable message
    const char* what() const;

  private:
   const int errorCode;
   std::shared_ptr<const std::string> message;
  };

}

#endif