#include <gtest/gtest.h>
#include <AtomError.h>

TEST(AtomErrorTest, Test_GetLogPath_Correct_CurDir) {
// TODO(Lawliet): Write here test code for *nix
  char curdir[MSG_BUFFER_SIZE];
  char curdir_new[MSG_BUFFER_SIZE];
#ifdef WINDOWS
  EXPECT_NO_THROW({
    GetCurrentDirectory(MSG_BUFFER_SIZE, curdir);
    AtomLog al;
    GetCurrentDirectory(MSG_BUFFER_SIZE, curdir_new);
    EXPECT_STREQ(curdir, curdir_new);
  });
#endif  // WINDOWS
#ifdef UNIX
  EXPECT_NO_THROW({
    getcwd(curdir, MSG_BUFFER_SIZE);
    AtomLog al;
    getcwd(curdir_new, MSG_BUFFER_SIZE);
    EXPECT_STREQ(curdir, curdir_new);
  });
#endif  // UNIX
}

TEST(AtomErrorTest, Test_GetLogPath) {
  char path[MSG_BUFFER_SIZE];
  EXPECT_NO_THROW({
    AtomLog al;
    EXPECT_STRNE(0, al.GetLogPath(path, MSG_BUFFER_SIZE));
  });
}
