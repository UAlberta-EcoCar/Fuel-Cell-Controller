#include "mbed.h"
#include "rtos.h"

#include "error_checking_thread.h"
#include "analog_read_thread.h"


void error_checking_thread(void const *args)
{
  while(true)
  {
    Thread::wait(5);
  }
}
