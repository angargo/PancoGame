#ifndef BASETOOLBAR_H
#define BASETOOLBAR_H

#include <gtkmm.h>

class BaseToolbar : public Gtk::Toolbar {
 private:
  static ModelColumns columns; 
   static Glib::RefPtr<Gtk::ListStore> refListStore =
      Gtk::ListStore::create(ModelColumns());

 static Glib::RefPtr<Gtk::ComboBox> refComboBox = 
   Gtk::ComboBox();
 static bool isComboInit = false; 
 public:
  BaseToolBar();
  virtual ~LeftPane();
};

#endif  // BASETOOLBAR_H
