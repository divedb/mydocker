#include "mydocker/cgroup/cgroup_manager.hh"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "mydocker/cgroup/memory.hh"

namespace fs = std::filesystem;
using mydocker::CgroupManager;
using mydocker::MemoryConfig;

class CgroupManagerTest : public ::testing::Test {
 protected:
  void SetUp() override { manager_ = std::make_unique<CgroupManager>(); }

  std::unique_ptr<CgroupManager> manager_;
};

TEST_F(CgroupManagerTest, CreateCgroup_Success) {
  std::string cg_name = "cg1";

  EXPECT_TRUE(manager_->CreateCgroup(cg_name));
  EXPECT_TRUE(fs::exists(manager_->CgroupMountPath() / cg_name));
}

TEST_F(CgroupManagerTest, CreateCgroup_AlreadyExists) {
  std::string cg_name = "cg2";
  fs::create_directory(manager_->CgroupMountPath() / cg_name);

  EXPECT_TRUE(manager_->CreateCgroup(cg_name));
  EXPECT_TRUE(fs::exists(manager_->CgroupMountPath() / cg_name));
}

TEST_F(CgroupManagerTest, RemoveCgroup_Success) {
  std::string cg_name = "cg3";

  EXPECT_TRUE(manager_->CreateCgroup(cg_name));
  EXPECT_TRUE(fs::exists(manager_->CgroupMountPath() / cg_name));

  manager_->RemoveCgroup(cg_name);
  EXPECT_FALSE(fs::exists(manager_->CgroupMountPath() / cg_name));
}

TEST_F(CgroupManagerTest, RemoveCgroup_NonExistent) {
  std::string cg_name = "cg4";
  // Should not throw or crash
  manager_->RemoveCgroup(cg_name);
}

TEST_F(CgroupManagerTest, ConfigureMemory_Success) {
  std::string cg_name = "cg5";

  EXPECT_TRUE(manager_->CreateCgroup(cg_name));

  MemoryConfig config;
  config.max_in_bytes = 1024 * 1024;

  EXPECT_TRUE(manager_->ConfigureMemory(cg_name, config));
}

TEST_F(CgroupManagerTest, ConfigureMemory_CgroupNotExist) {
  std::string cg_name = "cg6";
  MemoryConfig config;
  config.max_in_bytes = 1024 * 1024;

  EXPECT_FALSE(manager_->ConfigureMemory(cg_name, config));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}