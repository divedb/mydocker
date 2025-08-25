#pragma once

#include <expected>
#include <optional>
#include <string>
#include <vector>

namespace mydocker {

struct CommandLine {
  bool tty = false;
  bool interactive = false;
  std::vector<std::string> remaining_args;
};

std::optional<CommandLine> parse_command_line(int argc, char** argv);

}  // namespace mydocker