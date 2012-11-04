#include <gtest/gtest.h>
#ifdef WINDOWS
#include <conio.h>
#endif  // WWINDOWS
int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
#ifdef WINDOWS
  getch();
#endif  // WINDOWS
  return 0;
}