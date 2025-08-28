#pragma once

#include <optional>
#include <string>
#include <vector>

namespace mydocker {

struct CommandLine {
  bool tty = false;
  bool interactive = false;
  std::size_t mem_config = -1;
  std::vector<std::string> remaining_args;
};

std::optional<CommandLine> parse_command_line(int argc, char** argv);

}  // namespace mydocker