#include <gtest/gtest.h>
#include <AtomError.h>

TEST(AtomErrorTest, Test_GetLogPath_Correct_CurDir) {
// TODO(Lawliet): Write here test code for *nix
  char curdir[MAX_PATH];
  char curdir_new[MAX_PATH];
#ifdef WINDOWS
  EXPECT_NO_THROW({
    GetCurrentDirectory(MAX_PATH, curdir);
    AtomLog al;
    GetCurrentDirectory(MAX_PATH, curdir_new);
    EXPECT_STREQ(curdir, curdir_new);
  });
#endif  // WINDOWS
#ifdef UNIX
  EXPECT_NO_THROW({
    getcwd(curdir, MAX_PATH);
    AtomLog al;
    getcwd(curdir_new, MAX_PATH);
    EXPECT_STREQ(curdir, curdir_new);
  });
#endif  // UNIX
}

TEST(AtomErrorTest, Test_GetLogPath) {
  char path[MAX_PATH];
  EXPECT_NO_THROW({
    AtomLog al;
    EXPECT_STRNE(0, al.GetLogPath(path, MAX_PATH));
  });
}
