#ifndef CANVAS_H
#define CANVAS_H

#include <gtkmm.h>

class Canvas : public Gtk::DrawingArea {
 public:
  Canvas();
  virtual ~Canvas();
};

#endif  // CANVAS_H
