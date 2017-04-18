#include <iostream>
#include "movement.h"

movement:: movement() : active(false),  time_left(milliseconds(3600000)), 
			time_per_move(0) {}

void movement::start(steady_clock::time_point tp) {
  active.store(true);
  tick_thread = std::async(std::launch::async, &movement::ticking, this, tp);
}

void movement::stop(steady_clock::time_point tp) {
  active.store(false);
  milliseconds temp(duration_cast<milliseconds>(tp - tick_thread.get()));
  time_left.store(time_left.load() - temp);
  // active.store(false);
}

steady_clock::time_point movement::ticking(steady_clock::time_point tp) {
  while(steady_clock::now() < tp + time_per_move) {
    std::this_thread::sleep_for(seconds(1));
    if (!active.load()) return tp;
  }
  tp += time_per_move;
  milliseconds temp = time_left.load();
  while(1) {
    std::this_thread::sleep_for(temp - duration_cast<seconds>(temp) 
				+ milliseconds(1)); 
    steady_clock::time_point tp_temp = steady_clock::now();
    if (!active.load()) return tp_temp;
    temp -= duration_cast<milliseconds>(tp_temp - tp);
    time_left.store(temp);
    update.emit();
    if (temp.count() < 0)  {
      active.store(false);
      return tp_temp;
    }
    tp = tp_temp;
  }
}


    
  
