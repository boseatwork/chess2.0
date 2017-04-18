#ifndef GTKMM_EXAMPLE_CLOCK_H
#define GTKMM_EXAMPLE_CLOCK_H

#include <gtkmm/drawingarea.h>
#include "movement.h"

enum Players {white, black, white_pause, black_pause};
enum Kleppmode {untriggered, triggered, falling};

class Clock : public Gtk::DrawingArea, public movement {
public:
  Clock();
  ~Clock();
protected:
  //Override default signal handler:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
  void redraw();
  Kleppmode klepp;
  steady_clock::time_point tp_falling;
  double m_radius;
  double m_line_width;
};

#endif // GTKMM_EXAMPLE_CLOCK_H
