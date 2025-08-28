#pragma once
#include <string>

namespace mydocker {

class Status {
 public:
  static Status OK() { return Status(); }
  static Status Error(const std::string& msg) { return Status(false, msg); }

  bool HasError() const { return !ok_; }
  operator bool() const { return ok_; }
  const std::string& Message() const { return message_; }

 private:
  Status() : ok_(true) {}
  Status(bool ok, std::string msg) : ok_(ok), message_(std::move(msg)) {}

  bool ok_;
  std::string message_;
};

}  // namespace mydocker
