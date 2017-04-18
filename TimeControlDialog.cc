#include "Clockwindow.h"
#include <gtkmm/stock.h>
#include <iostream>
#include <gtkmm/button.h>

Clockwindow::TimeControlDialog::TimeControlDialog(Glib::ustring name):
  Dialog(name), time_per_game(0), 
  label1("Time per game (in format h:mm): "), label3(":"), label4(":"),
  label2("Time per move (in format m:ss): ") {
  entry1.set_text("1");
  entry1.set_width_chars(1);
  entry1.set_max_length(1);
  entry2.set_text("00");
  entry2.set_width_chars(2);
  entry2.set_max_length(2);
  entry3.set_text("0");
  entry3.set_width_chars(1);
  entry3.set_max_length(1);
  entry4.set_text("00");
  entry4.set_width_chars(2);
  entry4.set_max_length(2);
  hbox1.pack_start(label1, Gtk::PACK_SHRINK);
  hbox1.add(entry1);
  hbox1.add(label3);
  hbox1.add(entry2);
  hbox2.pack_start(label2, Gtk::PACK_SHRINK);
  hbox2.add(entry3);
  hbox2.add(label4);
  hbox2.add(entry4);
  get_vbox() -> pack_start(hbox1);
  get_vbox() -> add(hbox2);
  add_button(Gtk::StockID(Gtk::Stock::OK), Gtk::RESPONSE_OK);
  signal_response().connect
    (sigc::mem_fun(*this, &Clockwindow::TimeControlDialog::on_button_ok));

  show_all_children();
}


int Clockwindow::TimeControlDialog::string_to_int
(Glib::ustring en1, Glib::ustring en2) {
    int time;
    int temp = en1[0] - '0';
    if (!(0 < temp < 10)) {
      std::cerr << "Invalid input!" << std::endl;
      return 0;
    }
    time = 6*temp;
    temp = en2[0] - '0';
    if (!(0 < temp < 6)) {
      std::cerr << "Invalid input!" << std::endl;
      return 0;
    }
    time = 10*(temp + time);
    temp = en2[1] - '0';
    if (!(0 < temp < 10)) {
      std::cerr << "Invalid input!" << std::endl;
      return 0;
    }
    return time = time + temp;
  }

void Clockwindow::TimeControlDialog::on_button_ok(int i) {
  time_per_game = 
    milliseconds(60000 * string_to_int(entry1.get_text(), entry2.get_text()));
  time_per_move = 
    seconds(string_to_int(entry3.get_text(), entry4.get_text()));

  hide();
}
