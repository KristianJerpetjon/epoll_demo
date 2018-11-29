#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

#include <exception>
#include <functional>

#include <memory>
//#include <utility>

#define DEBUG

#define UNLIKELY(x) __builtin_expect(x,0)

//using namespace std;
//lolcode.. replace with delegate..
using CallBack=std::function<void(int)>;

//template<int fd>

#include "eventprocess.h"
#include "timerevent.h"
#include "event.h"


EventProcess ep;

void event()
{
  std::cout<<"Eventfd event triggered"<<std::endl;
}

Event testEvent(std::bind(event));

void timeout()
{
  std::cout<<"Timerfd event triggered"<<std::endl;
}

TimerEvent timerEvent(std::bind(timeout));

void ticks()
{
  static int i=0;
  i++;
  std::cout<<"Tick "<<i<<std::endl;

  if (i%5 == 0)
  {
    testEvent.sendEvent();
    printf("sendt a test event\n");
  }

  if (i%3 == 0)
  {

    //fire a timer event
    timerEvent.start();
    printf("started a timer\n");
  }

}

int main()
{
  ep.setTickFunction(std::bind(ticks));
  std::cout<<"Calling ep.run" << std::endl;
  timerEvent.setTimeout(50000);
  ep.addEvent(&timerEvent,EPOLLIN);
  ep.addEvent(&testEvent,EPOLLIN);
  ep.Run();
  std::cout << "Hello World!" << std::endl;
  return 0;
}
