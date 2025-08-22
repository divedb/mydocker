#include <CLI/CLI.hpp>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  CLI::App app;

  auto run = app.add_subcommand("run", "Run a container");
  bool tty = false;
  bool interactive = false;

  run->add_flag("-t", tty, "Allocate a pseudo-TTY");
  run->add_flag("-i", interactive, "Keep STDIN open");

  std::vector<std::string> remaining_args;
  run->add_option("command", remaining_args, "Command to run")
      ->expected(-1);  // 表示可以接收任意个参数

  CLI11_PARSE(app, argc, argv);

  if (*run) {
    std::cout << "Remaining arguments:\n";
    for (auto& arg : remaining_args) std::cout << "  " << arg << "\n";
  }
}
