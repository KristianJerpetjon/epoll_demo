#ifndef EPOLLEVENT_H
#define EPOLLEVENT_H

//TODO use delegates and remove virtual method!
class EpollEvent
{
public:
 // EpollEvent(CallBack &cb): method(cb) {}
  int getFd() { return fd; }
  virtual void execute()=0;// { method(fd); }
  //void setCallback(CallBack cb) { method=cb; }
protected:
  //CallBack method;
  int fd;
};
#endif // EPOLLEVENT_H
