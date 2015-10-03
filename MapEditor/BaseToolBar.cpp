#include "BaseToolbar.h"
#include "ModelColumns.h"

BaseToolbar::BaseToolbar(std::string name){
  Gtk::Toolbar();
  Gtk::TreeModel::iterator it = refListStore->append();
  Gtk::TreeModel::Row row = *it;
  row[columns.name] = name;
  row[columns.toolbar] = this;

  if(!isComboInit){
    refComboBox.packStart(columns.name);
    isComboInit = false;
  }

  this->add(*refComboBox);
}
   
