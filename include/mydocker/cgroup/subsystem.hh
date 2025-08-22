#pragma once

#include <string>

namespace mydocker {

class Subsystem {
 public:
  virtual ~Subsystem() = default;

  Subsystem(const Subsystem&) = default;
  Subsystem(Subsystem&&) = default;
  Subsystem& operator=(const Subsystem&) = default;
  Subsystem& operator=(Subsystem&&) = default;

  virtual bool Init() = 0;
  virtual bool Apply() = 0;
  virtual bool Reset() = 0;
  virtual const std::string& Name() const = 0;
};

}  // namespace mydocker