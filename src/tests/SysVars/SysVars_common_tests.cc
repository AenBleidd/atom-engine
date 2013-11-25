#include <gtest/gtest.h>
#include <SysVars/sysvars.h>
#include <SysVars/guid.h>

TEST(SysVarsTest, ByteOrderDetectTest) {
  // test ByteOrder
  EXPECT_NE(ByteOrderDetect(), -1);
  // test static variable and ByteOrder
  EXPECT_NE(ByteOrderDetect(), -1);
}

TEST(SysVarsTest, GUID_Tests) {
  char test_guid[] = "6F9619FF-8B86-D011-B42D-00CF4FC964FF";
  AGUID *pnew_aguid = GetAGUID(test_guid);
  EXPECT_NE(*(int*)pnew_aguid, 0);
  char *pnew_guid = GetCGUID(pnew_aguid);
  EXPECT_NE(*(int*)pnew_guid, 0);
  EXPECT_STRCASEEQ(pnew_guid, test_guid);
  delete pnew_aguid;
#ifdef WINDOWS
  RpcStringFree((RPC_CSTR*)&pnew_guid);
#endif  // WINDOWS
#ifdef UNIX
  delete [] pnew_guid;
#endif  // UNIX
}
