#include <gtkmm/messagedialog.h>
#include "Clockwindow.h"
#include <iostream>

Clockwindow::Clockwindow() : w_dial(), b_dial(), active(white_pause) {
  set_events(Gdk::BUTTON_PRESS_MASK);

  actions = Gtk::ActionGroup::create();
  actions -> add(Gtk::Action::create("timeControl","_Time control"));
  actions -> add(Gtk::Action::create("equalTime","_Equal time"),
		 sigc::mem_fun(*this, &Clockwindow::on_EqualTime));
  actions -> add(Gtk::Action::create("whiteTime", "_White Time"), 
		  sigc::mem_fun(*this, &Clockwindow::on_WhiteTime));  
  actions -> add(Gtk::Action::create("blackTime", "_Black Time"), 
		 sigc::mem_fun(*this, &Clockwindow::on_BlackTime));  

  // actions -> add(Gtk::Action::create("mode", "_Mode"));
  // actions -> add(Gtk::Action::create("analogue", "_Analogue"), 
  // 		 sigc::mem_fun(*this, &Clockwindow::on_Analogue));
  // actions -> add(Gtk::Action::create("digital", "_Digital"),
  // 		 sigc::mem_fun(*this, &Clockwindow::on_Digital));

  actions -> add(Gtk::Action::create("help", "_Help"));
  actions -> add(Gtk::Action::create("guide", "_Guide"), 
		 sigc::mem_fun(*this, &Clockwindow::on_Guide)); 
  actions -> add(Gtk::Action::create("about", "_About"), 
		 sigc::mem_fun(*this, &Clockwindow::on_About)); 

  uiManager = Gtk::UIManager::create();
  uiManager -> insert_action_group(actions);
  add_accel_group(uiManager -> get_accel_group());

  Glib::ustring ui_info =
    "<ui>"
    "  <menubar name='MenuBar'>"
    "    <menu action='timeControl'>"
    "      <menuitem action='equalTime'/>"
    "      <menuitem action='whiteTime'/>"
    "      <menuitem action='blackTime'/>"
    "    </menu>"
    // "    <menu action='mode'>"
    // "      <menuitem action='analogue'/>"
    // "      <menuitem action='digital'/>"
    // "    </menu>"
    "    <menu action='help' >"
    "      <menuitem action='guide'/>"
    "      <menuitem action='about'/>"
    "    </menu>"
    "  </menubar>"
    "</ui>";
  uiManager -> add_ui_from_string(ui_info);

  add(vbox);
  vbox.pack_start(*(uiManager -> get_widget("/MenuBar")), Gtk::PACK_SHRINK);
  vbox.add(hbox);
  hbox.pack_start(w_dial);
  // w_dial.show();
  hbox.pack_start(b_dial);
  // b_dial.show();
  // hbox.show();
  vbox.pack_start(hbox, Gtk::PACK_EXPAND_WIDGET);
  // vbox.show();
  show_all_children();
}

void Clockwindow::on_EqualTime() {
  on_user_interaction('p');
  active = white_pause; 
  TimeControlDialog dialog("Equal Time Dialogue");
  if (dialog.run() == Gtk::RESPONSE_OK) {
    milliseconds time = dialog.get_time();
    w_dial.set_time(time);
    b_dial.set_time(time);
    seconds te = dialog.get_time_per_move();
    w_dial.set_time_per_move(te);
    b_dial.set_time_per_move(te);
  }
}

void Clockwindow::on_WhiteTime() {
  TimeControlDialog dialog("White Time Dialogue") ;
  if (dialog.run() == Gtk::RESPONSE_OK) {
    w_dial.set_time(dialog.get_time());
    w_dial.set_time_per_move(dialog.get_time_per_move());
  }
}

void Clockwindow::on_BlackTime() {
  TimeControlDialog dialog("Black Time Dialogue");
  if (dialog.run() == Gtk::RESPONSE_OK) {
    b_dial.set_time(dialog.get_time());
    b_dial.set_time_per_move(dialog.get_time_per_move());
  }
}

// void Clockwindow::on_Digital() {
//   on_user_interaction('p');
//   vbox.remove(*clockface);
//   delete clockface;
//   clockface = new 
//     Clockface1(white_mechanism.get_time(), black_mechanism.get_time()); 
//   vbox.pack_start(*clockface, Gtk::PACK_EXPAND_WIDGET);
//   show_all();
// }

// void Clockwindow::on_Analogue() {
//   on_user_interaction('p');
//   vbox.remove(*clockface);
//   delete clockface;
//   clockface = new Clockface2
//     (white_mechanism.get_time(), black_mechanism.get_time());
//   vbox.pack_start(*clockface, Gtk::PACK_EXPAND_WIDGET);
//   show_all();
// }

void Clockwindow::on_Guide() {
  Gtk::MessageDialog dialogue(*this, "Guide");
  dialogue.set_secondary_text("Black may use the 'b' key, the 'Space' key or \
the right mouse button to maneuver the clock. Likewise, white may use the 'w' \
key, the 'Tab' key, or the left mouse button. Use the 'p' key if you \
wish to pause the game.");
  dialogue.run();
}

void Clockwindow::on_About() {
  Gtk::MessageDialog dialogue(*this, "About");
  dialogue.set_secondary_text("Björn Selander, December 2013");
  dialogue.run();
}

bool Clockwindow::on_button_press_event(GdkEventButton *button_event) {
  if (button_event -> button == 1) on_user_interaction('w');
  if (button_event -> button == 3) on_user_interaction('b');
  return true;
}

bool Clockwindow::on_key_press_event(GdkEventKey *key_event) {
  on_user_interaction(key_event -> keyval);
  return true;
}

void Clockwindow::on_user_interaction(char key) {
  using namespace std::chrono;
  steady_clock::time_point temp = steady_clock::now();
  switch(active) {
  case black:
    if (key == 'b' || key == ' ') {
      b_dial.stop(temp);
      w_dial.start(temp);  
      active = white;
    }
    if (key == 'p') {
      b_dial.stop(temp);
      active = black_pause;
    }
    break;
  case white:
    if (key == 'w' || key == '\t') {
      w_dial.stop(temp);
      b_dial.start(temp);  
      active = black;
    }
    if (key == 'p') {
      w_dial.stop(temp);
      active = white_pause;
    }
    break;
  case black_pause:
    if (key == 'w' || key == '\t') {
      b_dial.start(temp);  
      active = black;
    }
    break;
  case white_pause:
    if (key == 'b' || key == ' ') {
      w_dial.start(temp);
      active = white;
    }
    break;
  }
};

