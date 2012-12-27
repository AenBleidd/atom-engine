#include <gtest/gtest.h>
#include <AtomError.h>

TEST(AtomErrorTest, LoadStringsTest) {
  
  static char *codes[] = {
    "Test code 0",
    "Test code 1"
  };
  static char *descr = "Description";
  EXPECT_NO_THROW({
    AtomLog al;

    int code;
    EXPECT_GE((code = al.LoadStrings(ATOMERROR, codes, descr)), 0);
    al.SetLastErr(code, 0);
    ERR err0 = al.GetLastErr();
    EXPECT_EQ(err0.code, code);
    EXPECT_EQ(err0.sub_code, 0);
    al.SetLastErr(code, 1);
    ERR err1 = al.GetLastErr();
    EXPECT_EQ(err1.code, code);
    EXPECT_EQ(err1.sub_code, 1);

    int code1;
    EXPECT_GE((code1 = al.LoadStrings(ATOMWARNING, codes, descr)), 0);
    al.SetLastWrn(code1, 0);
    ERR err2 = al.GetLastWrn();
    EXPECT_EQ(err2.code, code1);
    EXPECT_EQ(err2.sub_code, 0);
    al.SetLastWrn(code1, 1);
    ERR err3 = al.GetLastWrn();
    EXPECT_EQ(err3.code, code1);
    EXPECT_EQ(err3.sub_code, 1);

  });
}
