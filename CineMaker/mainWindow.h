#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "canvas.h"

class MainWindow : public Gtk::Window
{
public:
  MainWindow();
  virtual ~MainWindow();

private:
  void filler();
  void on_menu_quit();
  
  Gtk::Box m_Box;
  Glib::RefPtr<Gtk::Builder> m_refBuilder;

};

#endif //MAINWINDOW_H
