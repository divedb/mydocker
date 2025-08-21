#include "cgroup_version.hh"

#include <gtest/gtest.h>

#include <string>

using mydocker::CgroupVersion;
using mydocker::ParseMountInfoLine;

TEST(CgroupVersionTest, ParseMountInfoLine_V1) {
  std::string line =
      "36 25 0:32 / /sys/fs/cgroup/cpu rw,nosuid,nodev,noexec,relatime - "
      "cgroup cgroup rw,cpu";
  EXPECT_EQ(ParseMountInfoLine(line), CgroupVersion::kV1);
}

TEST(CgroupVersionTest, ParseMountInfoLine_V2) {
  std::string line =
      "38 25 0:34 / /sys/fs/cgroup/unified rw,nosuid,nodev,noexec,relatime - "
      "cgroup2 cgroup rw";
  EXPECT_EQ(ParseMountInfoLine(line), CgroupVersion::kV2);
}

TEST(CgroupVersionTest, ParseMountInfoLine_UnknownFsType) {
  std::string line =
      "39 25 0:35 / /sys/fs/other rw,nosuid,nodev,noexec,relatime - tmpfs "
      "tmpfs rw";
  EXPECT_EQ(ParseMountInfoLine(line), CgroupVersion::kUnknown);
}

TEST(CgroupVersionTest, ParseMountInfoLine_InvalidFormat) {
  std::string line = "invalid mountinfo line";
  EXPECT_EQ(ParseMountInfoLine(line), CgroupVersion::kUnknown);
}

TEST(CgroupVersionTest, ParseMountInfoLine_EmptyString) {
  std::string line = "";
  EXPECT_EQ(ParseMountInfoLine(line), CgroupVersion::kUnknown);
}

TEST(CgroupVersionTest, ParseMountInfoLine_NoFsType) {
  std::string line =
      "36 25 0:32 / /sys/fs/cgroup/cpu rw,nosuid,nodev,noexec,relatime - ";
  EXPECT_EQ(ParseMountInfoLine(line), CgroupVersion::kUnknown);
}

// --- Note: Testing GetCgroupVersion() directly is not deterministic ---
// It reads the real /proc/self/mountinfo, so its result depends on the
// environment. For code coverage, you can run the test on systems with
// different cgroup setups. For full testability, consider refactoring
// GetCgroupVersion to accept an istream.

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}