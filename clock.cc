#include <cairomm/context.h>
#include <glibmm.h>
#include <cmath>
#include "clock.h"

// using namespace std;
using namespace std::chrono;

Clock::Clock() : m_radius(0.35), m_line_width(0.01), klepp(untriggered) {
  update.connect(sigc::mem_fun(*this, &Clock::redraw));
  #ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
  //Connect the signal handler if it isn't already a virtual method override:
  signal_draw().connect(sigc::mem_fun(*this, &Clock::on_draw), false);
  #endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
}

Clock::~Clock() {}

bool Clock::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
  const int scale = std::min(width, height);

  // scale to unit square and translate (0, 0) to be (0.5, 0.5), i.e.
  // the center of the window
  cr -> save();
  cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);   // green
  cr->paint();
  cr->restore();

  cr -> save();
  cr->translate(0.5*width, 0.5*height);  
  cr->scale(scale, scale);  
  cr->set_line_width(m_line_width);
  cr->arc(0, 0, m_radius, 0, 2 * M_PI);

  cr->save();
  cr->set_source_rgba(1.0, 1.0, 1.0, 0.8);
  cr->fill_preserve();
  cr->restore();
  cr->stroke_preserve();
  cr->clip();

  //clock ticks
  for (int i = 0; i < 12; i++)
    {
      double inset = 0.05;
	
      cr->save();
      cr->set_line_cap(Cairo::LINE_CAP_ROUND);
	
      if(i % 3 != 0)
	{
	  inset *= 0.8;
	  cr->set_line_width(0.005);
	}
	
      cr->move_to(
		  (m_radius - inset) * cos (i * M_PI / 6),
		  (m_radius - inset) * sin (i * M_PI / 6));
      cr->line_to (
		   m_radius * cos (i * M_PI / 6),
		   m_radius * sin (i * M_PI / 6));
      cr->stroke();
      cr->restore(); /* stack-pen-size */
    }

  // Get remaining time
  milliseconds remains = time();

  // compute the angles of the indicators of our clock
  int secs = duration_cast<seconds>(remains+milliseconds(5)).count(); 
    float hours = secs/3600.0;
    float min = secs/60.0;

    std::cout << " Milliseconds: " << remains.count() << std::endl;
    
    cr->save();
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);

    // draw the seconds hand
    cr->save();
    cr->set_line_width(m_line_width / 3);
    cr->set_source_rgba(0.7, 0.7, 0.7, 0.8); // gray
    cr->move_to(0, 0);
    cr->line_to(sin(M_PI*secs/30.0) * (m_radius * 0.9),
		-cos(M_PI*secs/30.0) * (m_radius * 0.9));
    cr->stroke();
    cr->restore();

    // draw the minutes hand
    cr->set_source_rgba(0.117, 0.337, 0.612, 0.9);   // blue
    cr->move_to(0, 0);
    float x_min = sin(M_PI*min/30.0) * (m_radius * 0.8);
    float y_min = -cos(M_PI*min/ 30) * (m_radius * 0.8); 
    cr->line_to(x_min, y_min);
    cr->stroke();

    // draw the hours hand
    cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);   // green
    cr->move_to(0, 0);
    cr->line_to(sin(M_PI*hours/ 6.0) * (m_radius * 0.5),
		-cos(M_PI*hours/ 6.0) * (m_radius * 0.5));
    cr->stroke();
    cr->restore();

    // draw the klepp
    cr -> save();
    cr -> set_line_width(m_line_width/2);
    cr -> set_source_rgba(1, 0, 0, 0.9);
    double klepp_length = sqrt(1.64 - 1.6 * cos(M_PI/30))*m_radius;
    double x_klepp = sin(M_PI/30)*m_radius;
    double y_klepp = -cos(M_PI/30)*m_radius;
    cr -> move_to(x_klepp, y_klepp);
    switch (klepp) {
    case untriggered:
      cr -> rel_line_to(0, klepp_length);      
      if (0 < x_min && x_min * (y_klepp + klepp_length) > y_min * x_klepp) 
	klepp = triggered;
      break;
    case triggered:
      {
	double len(std::sqrt(std::pow(x_klepp - x_min, 2) + std::pow(y_klepp - y_min, 2)));
	cr -> rel_line_to(klepp_length*(x_min - x_klepp)/len, 
			  -klepp_length * (y_klepp - y_min)/len);
      }
      if (min <= 0) {
	klepp = falling;
	tp_falling = steady_clock::now();
	this -> on_draw(cr);
      }
      break;
    case falling:
      double temp = duration_cast<milliseconds>
	(steady_clock::now() - tp_falling).count()/1000.0;

      double angle = exp(-temp)*asin(x_klepp/klepp_length);
      cr -> rel_line_to(klepp_length*sin(angle*cos(3.1*temp)), 
			    klepp_length*cos(angle*cos(3.1*temp)));
      cr -> stroke();
      cr -> move_to(x_klepp, y_klepp);
      if (temp > 5) klepp = untriggered;
      int half_width = get_allocation().get_width()/2;
      Gdk::Rectangle rec(0, 0, get_allocation().get_width(), 
			 get_allocation().get_height());
      get_window() -> invalidate_rect(rec, false);
      break;
    }
    std::cout << "Klepp: " << klepp << std::endl;
    cr -> stroke();
    cr -> restore();

    // draw a little dot in the middle
    cr->arc(0, 0, m_line_width / 3.0, 0, 2 * M_PI);
    cr->fill();
    cr -> restore();

  return true;
}

void Clock::redraw() {
  // force our program to redraw the entire clock.
  Glib::RefPtr<Gdk::Window> win = get_window();
  if (win) {
      Gdk::Rectangle r(0, 0, get_allocation().get_width(),
		       get_allocation().get_height());
      win->invalidate_rect(r, false);
    }
}
