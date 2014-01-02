#include <gtest/gtest.h>
#include <AtomLog.h>
#include <atomfs.h>

TEST(AtomFSTest, AtomFS_Create_Test)
{
    AtomLog al;
    AtomFS afs(&al);
    char *file = "AtomFS_Create_Test.tmp";
    char *file000 = "AtomFS_Create_Test.tmp.000";
    char *file001 = "AtomFS_Create_Test.tmp.001";

    char *folder = "log";
    char **pfolder = &folder;

    HANDLE hOutputFile;

    EXPECT_FALSE(afs.Create(pfolder, 0, file));
    hOutputFile = CreateFile(file, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    EXPECT_EQ(INVALID_HANDLE_VALUE, hOutputFile);

    EXPECT_FALSE(afs.Create(0, 1, file));
    hOutputFile = CreateFile(file, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    EXPECT_EQ(INVALID_HANDLE_VALUE, hOutputFile);

    EXPECT_TRUE(afs.Create(0, 0, file));
    hOutputFile = CreateFile(file, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    EXPECT_NE(INVALID_HANDLE_VALUE, hOutputFile);
    EXPECT_NO_THROW(CloseHandle(hOutputFile));

    EXPECT_TRUE(afs.Create(0, 0, file));
    hOutputFile = CreateFile(file, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    EXPECT_NE(INVALID_HANDLE_VALUE, hOutputFile);
    EXPECT_NO_THROW(CloseHandle(hOutputFile));
    hOutputFile = CreateFile(file000, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    EXPECT_NE(INVALID_HANDLE_VALUE, hOutputFile);
    EXPECT_NO_THROW(CloseHandle(hOutputFile));

    EXPECT_TRUE(afs.Create(0, 0, file));
    hOutputFile = CreateFile(file, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    EXPECT_NE(INVALID_HANDLE_VALUE, hOutputFile);
    EXPECT_NO_THROW(CloseHandle(hOutputFile));
    hOutputFile = CreateFile(file000, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    EXPECT_NE(INVALID_HANDLE_VALUE, hOutputFile);
    EXPECT_NO_THROW(CloseHandle(hOutputFile));
    hOutputFile = CreateFile(file001, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    EXPECT_NE(INVALID_HANDLE_VALUE, hOutputFile);
    EXPECT_NO_THROW(CloseHandle(hOutputFile));

    EXPECT_NO_THROW({
      EXPECT_TRUE(DeleteFile(file));
      EXPECT_TRUE(DeleteFile(file000));
      EXPECT_TRUE(DeleteFile(file001));
    });

    EXPECT_TRUE(afs.Create(pfolder, 1, file));
    hOutputFile = CreateFile(file, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    EXPECT_NE(INVALID_HANDLE_VALUE, hOutputFile);
    EXPECT_NO_THROW(CloseHandle(hOutputFile));
    EXPECT_NO_THROW({
      EXPECT_TRUE(DeleteFile(file));
    });
}