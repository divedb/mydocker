#pragma once

#include <string>

namespace mydocker {

class CgroupManager {
 public:
  CgroupManager();
  [[no_discard]] bool IsValid() const;
  [[no_discard]] bool CreateDir(const std::string& dir);

 private:
  bool is_valid_;
};

}  // namespace mydocker