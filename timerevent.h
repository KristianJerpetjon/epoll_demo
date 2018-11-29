#ifndef TIMEREVENT_H
#define TIMEREVENT_H

#include "epollevent.h"

#include <sys/timerfd.h>
#include <fcntl.h>

class TimerEvent : public EpollEvent
{
public:
  TimerEvent(CallBack cb) : cb(cb)
  {
    fd = timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
    if (fd < 0)
    {
      throw std::runtime_error("Failed to create timerfd");
    }
  }
  //TODO make periodic a variable
  //TODO eat std::chrono ?
  void setTimeout(uint32_t us,bool periodic=false)
  {
    if (periodic)
    {
      its.it_interval.tv_sec=0;
      its.it_interval.tv_nsec=1000*us;
    }
    else
    {
      its.it_interval.tv_sec=0;
      its.it_interval.tv_nsec=0;
    }
    its.it_value.tv_sec=0;
    its.it_value.tv_nsec=1000*us;
  }
  void start(uint32_t us=0,bool periodic=false)
  {
    if (us != 0)
    {
      its.it_value.tv_sec=0;
      its.it_value.tv_nsec=1000*us;
    }

    timerfd_settime(fd,0,&its,NULL);

  }
  void stop()
  {
    its.it_value.tv_sec=0;
    its.it_value.tv_nsec=0;
    timerfd_settime(fd,0,&its,NULL);
  }

private:
  CallBack cb;
  struct itimerspec its;
  void execute() override
  {
    uint64_t expirations;
    if (read(fd,&expirations,8) == 8)
    {
      cb(fd);
    }
  }
};
#endif // TIMEREVENT_H
