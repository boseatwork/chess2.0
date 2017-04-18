#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/window.h>
#include <gtkmm/menubar.h>
#include <gtkmm/menu.h>
#include <gtkmm/box.h>
#include <gtkmm/uimanager.h>
#include "clock.h"

class Clockwindow : public Gtk::Window {
 public:
  Clockwindow();
 private:
  Clock w_dial, b_dial;
  Players active;
  Gtk::VBox vbox;
  Gtk::HBox hbox;

  // User interface when running. Overriding Gtk::Widget virtual fcns.
  bool on_key_press_event(GdkEventKey *); 
  bool on_button_press_event(GdkEventButton *);
  void on_user_interaction(char);

  /* The rest is devoted to the dialogues for adjusting the time controls. */

  Glib::RefPtr<Gtk::ActionGroup> actions;
  Glib::RefPtr<Gtk::UIManager> uiManager;

  void on_EqualTime(); //Call-backs.
  void on_WhiteTime();
  void on_BlackTime();
  // void on_Digital();
  // void on_Analogue();
  void on_Guide();
  void on_About();

  class TimeControlDialog : public Gtk::Dialog {
  public:
    TimeControlDialog(Glib::ustring);
    std::chrono::milliseconds get_time() {return time_per_game;}
    std::chrono::seconds get_time_per_move() {return time_per_move;}
  private:
    Gtk::HBox hbox1, hbox2;
    Gtk::Label label1, label2, label3, label4;
    Gtk::Entry entry1, entry2, entry3, entry4;
    Gtk::Button *ok;
    void on_button_ok(int);
    std::chrono::milliseconds time_per_game;
    std::chrono::seconds time_per_move;
    int string_to_int(Glib::ustring , Glib::ustring );
  };
};

#endif
