#ifndef EVENT_H
#define EVENT_H

#include "epollevent.h"

#include <sys/eventfd.h>

class Event : public EpollEvent
{
public:
  //allow callback to be registered
  Event(CallBack cb) : cb(cb)
  {
    fd = eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC);
    if (fd < 0)
    {
      throw std::runtime_error("Failed to create eventfd");
    }
  }
  void sendEvent()
  {
    uint64_t event=1;
    //should check return etc.. but you are not wrapping any time soon
    write(fd,&event,8);
  }
private:
  CallBack cb;
  void execute() override {
    uint64_t counter;
    if (read(fd,&counter,8) == 8)
    {
      //yay we got a counter..
      //issue callback
      cb(fd);
    }
    //throw if not ?

  }

};
#endif // EVENT_H
