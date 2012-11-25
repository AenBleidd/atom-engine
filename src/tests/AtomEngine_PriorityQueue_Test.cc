#include <gtest/gtest.h>
#include <AtomEngine.h>

TEST(AtomEngine, TestCorrectPriority) {
  AtomEngine ae;
  const unsigned char prior1 = 2, prior2 = 3, prior3 = 5, prior4 = 7;
  ae.SendMessage(0, NON_TYPE_MODULE, SEND_TO_ALL, 0, prior3, 0, 0);
  ae.SendMessage(0, NON_TYPE_MODULE, SEND_TO_ALL, 0, prior4, 0, 0);
  ae.SendMessage(0, NON_TYPE_MODULE, SEND_TO_ALL, 0, prior2, 0, 0);
  ae.SendMessage(0, NON_TYPE_MODULE, SEND_TO_ALL, 0, prior1, 0, 0);
  int32_t result;
  if (!ae.msg_queue.empty()) {
    MESSAGE_QUEUE mq = ae.msg_queue.top();
    ae.msg_queue.pop();
    result = (int32_t)mq.prior;
  } else {
    result = 0;
  }
  EXPECT_EQ(prior1, result);
  if (!ae.msg_queue.empty()) {
    MESSAGE_QUEUE mq = ae.msg_queue.top();
    ae.msg_queue.pop();
    result = (int32_t)mq.prior;
  } else {
    result = 0;
  }
  EXPECT_EQ(prior2, result);
  if (!ae.msg_queue.empty()) {
    MESSAGE_QUEUE mq = ae.msg_queue.top();
    ae.msg_queue.pop();
    result = (int32_t)mq.prior;
  } else {
    result = 0;
  }
  EXPECT_EQ(prior3, result);
  if (!ae.msg_queue.empty()) {
    MESSAGE_QUEUE mq = ae.msg_queue.top();
    ae.msg_queue.pop();
    result = (int32_t)mq.prior;
  } else {
    result = 0;
  }
  EXPECT_EQ(prior4, result);
  if (!ae.msg_queue.empty()) {
    MESSAGE_QUEUE mq = ae.msg_queue.top();
    ae.msg_queue.pop();
    result = (int32_t)mq.prior;
  } else {
    result = 0;
  }
// there is no element left
  EXPECT_EQ(0, result);
}
