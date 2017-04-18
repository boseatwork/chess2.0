#ifndef MOVEMENT_H
#define MOVEMENT_H

#include<chrono>
#include<atomic>
#include<memory>
#include<iostream>
#include<thread>
#include<future>
#include <glibmm/dispatcher.h>
#include <sigc++/sigc++.h>

using namespace std::chrono;

// class Clock;

class movement {
 public:
  movement();
  ~movement() {active.store(false);}
  milliseconds time() {return time_left.load();}
  void set_time(milliseconds te) {time_left.store(te);}
  void set_time_per_move(seconds ti) {time_per_move = ti;}
  void start(steady_clock::time_point);
  void stop(steady_clock::time_point);  
private:
  std::atomic_bool active;
  std::atomic<milliseconds> time_left;
  seconds time_per_move;
  std::future<steady_clock::time_point> tick_thread;
  steady_clock::time_point ticking(steady_clock::time_point);
 protected:
  Glib::Dispatcher update;
};

#endif // MOVEMENT_H
