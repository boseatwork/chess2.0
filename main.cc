#include "Clockwindow.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>

int main(int argc, char** argv)
{
   Glib::RefPtr<Gtk::Application> app 
     = Gtk::Application::create(argc, argv, "org.gtkmm.example");

   Clockwindow win;
   win.set_title("Chess clock");
   win.set_default_size(700,500);

   return app->run(win);
   // mm .join();
}
