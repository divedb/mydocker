#include "mydocker/common/cmd.hh"

#include <CLI/CLI.hpp>

namespace mydocker {

std::optional<CommandLine> parse_command_line(int argc, char** argv) {
  CLI::App app;
  CommandLine result;

  auto run = app.add_subcommand("run", "Run a container");

  run->add_flag("-t", result.tty, "Allocate a pseudo-TTY");
  run->add_flag("-i", result.interactive, "Keep STDIN open");
  run->add_flag("-m", result.mem_config, "Set memory limit");
  run->add_option("command", result.remaining_args, "Command to run")
      ->expected(-1);

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError& e) {
    return std::nullopt;
  }

  return result;
}

}  // namespace mydocker