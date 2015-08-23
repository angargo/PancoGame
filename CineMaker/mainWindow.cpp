#include "mainWindow.h"
#include <iostream>

MainWindow::MainWindow()
: m_Box(Gtk::ORIENTATION_VERTICAL)
{
  set_title("CineMaker: the beginnings");
  set_size_request(800,600);
  set_border_width(6);

  add(m_Box);

  //create actions
  Glib::RefPtr<Gio::SimpleActionGroup> refActionGroup = 
     Gio::SimpleActionGroup::create();

  refActionGroup->add_action("newstandard",
     sigc::mem_fun(*this, &MainWindow::filler));
  refActionGroup->add_action("new2",
     sigc::mem_fun(*this, &MainWindow::filler));

  refActionGroup->add_action("quit",
     sigc::mem_fun(*this, &MainWindow::on_menu_quit));

  refActionGroup->add_action("hulp",
     sigc::mem_fun(*this, &MainWindow::filler));

  insert_action_group("menu_actions", refActionGroup);
  m_refBuilder = Gtk::Builder::create();

  //Layout
  Glib::ustring ui_info = 
  "<interface>"
  "  <menu id='top-menu'>"
  "    <submenu>"
  "      <attribute name='label'>_File</attribute>"
  "      <section>"
  "        <item>"
  "          <attribute name='label'>New _Std</attribute>"
  "          <attribute name='action'>menu_actions.newstandard</attribute>"
  "          <attribute name='accel'>&lt;Primary&gt;n</attribute>"
  "        </item>"
  "        <item>"
  "          <attribute name='label'>New _Alt</attribute>"
  "          <attribute name='action'>menu_actions.new2</attribute>"
  "        </item>"
  "      </section>"
  "      <section>"
  "        <item>"
  "          <attribute name='label'>_Quit</attribute>"
  "          <attribute name='action'>menu_actions.quit</attribute>"
  "          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
  "        </item>"
  "      </section>"
  "    </submenu>"
  "    <submenu>"
  "      <attribute name='label'>_About</attribute>"
  "      <section>"
  "        <item>"
  "          <attribute name='label'>_Help</attribute>"
  "          <attribute name='action'>menu_actions.hulp</attribute>"
  "          <attribute name='accel'>&lt;Primary&gt;h</attribute>"
  "        </item>"
  "      </section>"
  "    </submenu>"
  "  </menu>"
  "</interface>";

  try{
    m_refBuilder->add_from_string(ui_info);
  }
  catch(const Glib::Error& ex){
    std::cerr << "building menus failed: " << ex.what();
  }

  //get menubar
  Glib::RefPtr<Glib::Object> object =
     m_refBuilder->get_object("top-menu");
  Glib::RefPtr<Gio::Menu> gmenu = 
     Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
  if(!gmenu)
    g_warning("GMenu not found");
  
  //put it there
  Gtk::MenuBar* pMenubar = Gtk::manage(new Gtk::MenuBar(gmenu));
  m_Box.pack_start(*pMenubar, Gtk::PACK_SHRINK);

  //make toolbar
  Gtk::Toolbar* toolbar = Gtk::manage(new Gtk::Toolbar());
  Gtk::ToolButton* button = Gtk::manage(new Gtk::ToolButton());
  button->set_icon_name("document-new");
  gtk_actionable_set_detailed_action_name (
    GTK_ACTIONABLE (button->gobj()), "menu_actions.newstandard");
  toolbar->add(*button);
  button = Gtk::manage(new Gtk::ToolButton());
  button->set_icon_name("exit");
  gtk_actionable_set_detailed_action_name (
    GTK_ACTIONABLE (button->gobj()), "menu_actions.quit");
  toolbar->add(*button);
  
  m_Box.pack_start(*toolbar, Gtk::PACK_SHRINK);

  show_all_children();
}

MainWindow::~MainWindow(){
  std::cout << "destroying window" << std::endl;
}

void MainWindow::filler(){
  std::cout << "HUEHUEHEUHEUE!!!" << std::endl;
}

void MainWindow::on_menu_quit(){
  hide();
}
