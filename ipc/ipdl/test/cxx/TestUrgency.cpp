#include "TestUrgency.h"

#include "IPDLUnitTests.h"      // fail etc.
#include <unistd.h>
#if !defined(OS_POSIX)
#include <windows.h>
#endif

template<>
struct RunnableMethodTraits<mozilla::_ipdltest::TestUrgencyParent>
{
    static void RetainCallee(mozilla::_ipdltest::TestUrgencyParent* obj) { }
    static void ReleaseCallee(mozilla::_ipdltest::TestUrgencyParent* obj) { }
};

namespace mozilla {
namespace _ipdltest {

#if defined(OS_POSIX)
static void Sleep(int ms)
{
    sleep(ms / 1000);
}
#endif

//-----------------------------------------------------------------------------
// parent

TestUrgencyParent::TestUrgencyParent()
  : inreply_(false)
{
    MOZ_COUNT_CTOR(TestUrgencyParent);
}

TestUrgencyParent::~TestUrgencyParent()
{
    MOZ_COUNT_DTOR(TestUrgencyParent);
}

void
TestUrgencyParent::Main()
{
  if (!SendStart())
    fail("sending Start");
}

bool
TestUrgencyParent::RecvTest1(uint32_t *value)
{
  if (!CallReply1(value))
    fail("sending Reply1");
  if (*value != 99)
    fail("bad value");
  return true;
}

bool
TestUrgencyParent::RecvTest2()
{
  uint32_t value;
  inreply_ = true;
  if (!CallReply2(&value))
    fail("sending Reply2");
  inreply_ = false;
  if (value != 500)
    fail("bad value");
  return true;
}

bool
TestUrgencyParent::RecvTest3(uint32_t *value)
{
  if (inreply_)
    fail("nested non-urgent on top of urgent rpc");
  *value = 1000;
  return true;
}

bool
TestUrgencyParent::RecvFinalTest_Begin()
{
  SetReplyTimeoutMs(2000);
  if (CallFinalTest_Hang())
    fail("should have failed due to timeout");
  if (!GetIPCChannel()->Unsound_IsClosed())
    fail("channel should have closed");

  MessageLoop::current()->PostTask(
      FROM_HERE,
      NewRunnableMethod(this, &TestUrgencyParent::Close));
  return false;
}

//-----------------------------------------------------------------------------
// child

enum {
  kFirstTestBegin = 1,
  kFirstTestGotReply,
  kSecondTestBegin,
  kSecondTestGotReply,
};

bool
TestUrgencyChild::RecvStart()
{
  uint32_t result;
  
  // Send a synchronous message, expect to get an urgent message while
  // blocked.
  test_ = kFirstTestBegin;
  if (!SendTest1(&result))
    fail("calling SendTest1");
  if (result != 99)
    fail("bad result in RecvStart");
  if (test_ != kFirstTestGotReply)
    fail("never received urgent message");

  // Initiate the next test by sending an asynchronous message, then becoming
  // blocked. This tests that the urgent message is still delivered properly,
  // and that the parent does not try to service the sync 
  test_ = kSecondTestBegin;
  if (!SendTest2())
    fail("calling SendTest2");
  if (!SendTest3(&result))
    fail("calling SendTest3");
  if (test_ != kSecondTestGotReply)
    fail("never received urgent message #2");
  if (result != 1000)
    fail("wrong value from test3");

  // This must be the last test, since the child process may die.
  if (SendFinalTest_Begin())
    fail("Final test should not have succeeded");

  Close();

  return true;
}

bool
TestUrgencyChild::AnswerReply1(uint32_t *reply)
{
  if (test_ != kFirstTestBegin)
    fail("wrong test # in AnswerReply1");

  *reply = 99;
  test_ = kFirstTestGotReply;
  return true;
}

bool
TestUrgencyChild::AnswerReply2(uint32_t *reply)
{
  if (test_ != kSecondTestBegin)
    fail("wrong test # in AnswerReply2");

  // sleep for 5 seconds so the parent process tries to deliver more messages.
  Sleep(5000);

  *reply = 500;
  test_ = kSecondTestGotReply;
  return true;
}

bool
TestUrgencyChild::AnswerFinalTest_Hang()
{
  sleep(10);
  return true;
}

TestUrgencyChild::TestUrgencyChild()
  : test_(0)
{
    MOZ_COUNT_CTOR(TestUrgencyChild);
}

TestUrgencyChild::~TestUrgencyChild()
{
    MOZ_COUNT_DTOR(TestUrgencyChild);
}

} // namespace _ipdltest
} // namespace mozilla
