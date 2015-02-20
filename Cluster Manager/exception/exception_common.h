#ifndef EXCEPTION_COMMON
#define EXCEPTION_COMMON

#include <string>

namespace exception {

  /// Formats message for given error
  std::string formatMessage(const int errorCode);

  /// Containst error code. Contains readable message of error.
  class Exception : public std::runtime_error {
  public:
    explicit Exception(const int errorCode);
    virtual ~Exception();

    /// Provide error code
    const int getErrorCode() const;

  private:
   const int errorCode;
  };

}

#endif