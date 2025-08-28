#include "mydocker/common/cmd.hh"

#include <gtest/gtest.h>

#include <optional>

using namespace mydocker;

TEST(ParseCommandLineTest, RunWithNoFlags) {
  const char* argv[] = {"mydocker", "run", "echo", "hello"};
  int argc = sizeof(argv) / sizeof(argv[0]);

  auto result = parse_command_line(argc, const_cast<char**>(argv));

  ASSERT_TRUE(result.has_value());
  EXPECT_FALSE(result->tty);
  EXPECT_FALSE(result->interactive);
  EXPECT_FALSE(result->mem_config);
  ASSERT_EQ(result->remaining_args.size(), 2);
  EXPECT_EQ(result->remaining_args[0], "echo");
  EXPECT_EQ(result->remaining_args[1], "hello");
}

TEST(ParseCommandLineTest, RunWithTtyAndInteractiveFlags) {
  const char* argv[] = {"mydocker", "run", "-t", "-i", "bash"};
  int argc = sizeof(argv) / sizeof(argv[0]);

  auto result = parse_command_line(argc, const_cast<char**>(argv));

  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(result->tty);
  EXPECT_TRUE(result->interactive);
  EXPECT_FALSE(result->mem_config);
  ASSERT_EQ(result->remaining_args.size(), 1);
  EXPECT_EQ(result->remaining_args[0], "bash");
}

TEST(ParseCommandLineTest, RunWithMemoryFlag) {
  const char* argv[] = {"mydocker", "run", "-m", "512m", "myapp"};
  int argc = sizeof(argv) / sizeof(argv[0]);

  auto result = parse_command_line(argc, const_cast<char**>(argv));

  ASSERT_TRUE(result.has_value());
  EXPECT_FALSE(result->tty);
  EXPECT_FALSE(result->interactive);
  EXPECT_TRUE(result->mem_config);
  ASSERT_EQ(result->remaining_args.size(), 2);
  EXPECT_EQ(result->remaining_args[0], "512m");
  EXPECT_EQ(result->remaining_args[1], "myapp");
}

TEST(ParseCommandLineTest, InvalidCommand) {
  const char* argv[] = {"mydocker", "invalidcmd"};
  int argc = sizeof(argv) / sizeof(argv[0]);

  auto result = parse_command_line(argc, const_cast<char**>(argv));

  EXPECT_FALSE(result.has_value());
}

TEST(ParseCommandLineTest, RunWithAllFlagsAndArgs) {
  const char* argv[] = {"mydocker", "run", "-t",     "-i",
                        "-m",       "1G",  "python", "script.py"};
  int argc = sizeof(argv) / sizeof(argv[0]);

  auto result = parse_command_line(argc, const_cast<char**>(argv));

  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(result->tty);
  EXPECT_TRUE(result->interactive);
  EXPECT_TRUE(result->mem_config);
  ASSERT_EQ(result->remaining_args.size(), 3);
  EXPECT_EQ(result->remaining_args[0], "1G");
  EXPECT_EQ(result->remaining_args[1], "python");
  EXPECT_EQ(result->remaining_args[2], "script.py");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}