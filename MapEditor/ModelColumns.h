#ifndef COLUMNS_H
#define COLUMNS_H

#include <gtkmm.h>

class ModelColumns : public TreeModelColumnRecord {
 public:
  ModelColumns() {
    add(name);
    add(toolbar);
  }
  Gtk::TreeModelColumn<Glib::ustring> name;
  Gtk::TreeModelColumn<*BaseToolbar> toolbar;
};

#endif  // COLUMNS_H
