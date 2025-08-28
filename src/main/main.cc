#include <glog/logging.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "mydocker/cgroup/cgroup_manager.hh"
#include "mydocker/common/cmd.hh"

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  auto cmd = mydocker::parse_command_line(argc, argv);

  std::cout << "what happened\n";

  if (!cmd.has_value()) {
    std::cout << "invalid usage\n";
    return 1;
  }

  pid_t pid = fork();

  if (pid < 0) perror("fork");

  if (pid == 0) {
    mydocker::CgroupManager cm;

    exit(2);
  }

  int status;
  waitpid(pid, &status, 0);
}
