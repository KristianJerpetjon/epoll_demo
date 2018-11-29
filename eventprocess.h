#ifndef EPOLLPROCESS_H
#define EPOLLPROCESS_H

#include "epollevent.h"

class EventProcess
{
public:
  //default is 1 second tick..
  EventProcess(CallBack tick=nullptr,int max_events=10,int timeout=1000) :
    events(new struct epoll_event[max_events]),
    timeout(timeout),
    max_events(max_events)
  {
    epoll_fd=epoll_create1(0);
  }

  ~EventProcess()
  {
    //if thread running kill thread
    //we cant throw so no point checking return.
    close(epoll_fd);
  }
  void setTickFunction(std::function<void()> cb) { tick=cb;}
  //downcasting 4tw
  void addEvent(EpollEvent *ev,int events)
  {
    struct epoll_event event;
    event.events=events;
    //notice the union in epoll_data
    event.data.u64=(uint64_t)(ev);
    if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,ev->getFd(),&event) != 0)
    {
      throw std::runtime_error("Failed to add fd");
    }
  }
  void removeEvent(EpollEvent *ev)
  {
    if (epoll_ctl(epoll_fd,EPOLL_CTL_DEL,ev->getFd(),NULL) != 0)
    {
      throw std::runtime_error("Failed to remvoe fd");
    }
  }
  //could have returned error ?
  void Run() //Todo do a threaded and non threaded version..
  {
    while(1)
    {
      struct epoll_event *event_list=events.get();
      int count=epoll_wait(epoll_fd,event_list,max_events,timeout);
      if (UNLIKELY(count <= 0))
      {
        if (UNLIKELY(count < 0))
        {
          //something really bad has happened
        }
        else
        {
            //run CB on tick on tick!!
            if (tick != nullptr)
            {
              tick();
            }
        }
      }
      else
      {
        //call event function in all registered events
        for (int i =0;i<count;i++)
        {
          EpollEvent *ev=(EpollEvent *)event_list[i].data.u64;
          ev->execute();
        }
      }
    }
  }
private:
  std::unique_ptr<struct epoll_event[]> events;
  int timeout;
  int max_events;
  int epoll_fd;
  std::function<void()> tick=nullptr;
};

#endif // EPOLLPROCESS_H
