#include <gtest/gtest.h>
#include <AtomLog.h>
#include <atomfs.h>

TEST(AtomFSTest, AtomFS_Common_Test)
{
  EXPECT_NO_THROW({
    AtomLog al;
    AtomFS afs(&al);
  });
}