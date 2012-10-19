#include <gtest\gtest.h>
#include <AtomEngine.h>

#ifdef ATOM_DEBUG
// we can run this test only in debug mode
TEST(PriorityQueue, TestCorrectPriority) {
  AtomEngine ae;
  const unsigned char prior1 = 2, prior2 = 3, prior3 = 5, prior4 = 7;
  ae.SendMessage(0, NON_TYPE_MODULE, SEND_TO_ALL, 0, prior3, 0, 0);
  ae.SendMessage(0, NON_TYPE_MODULE, SEND_TO_ALL, 0, prior4, 0, 0);
  ae.SendMessage(0, NON_TYPE_MODULE, SEND_TO_ALL, 0, prior2, 0, 0);
  ae.SendMessage(0, NON_TYPE_MODULE, SEND_TO_ALL, 0, prior1, 0, 0);

  EXPECT_EQ(prior1, ae.GetMessage(0, 0, 0, 0, 0));
  EXPECT_EQ(prior2, ae.GetMessage(0, 0, 0, 0, 0));
  EXPECT_EQ(prior3, ae.GetMessage(0, 0, 0, 0, 0));
  EXPECT_EQ(prior4, ae.GetMessage(0, 0, 0, 0, 0));
// there is no element left
  EXPECT_EQ(0, ae.GetMessage(0, 0, 0, 0, 0));
}
#endif
