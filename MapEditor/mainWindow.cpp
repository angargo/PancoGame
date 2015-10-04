#include "mainWindow.h"
#include <iostream>

MainWindow::MainWindow()
    : m_Box(Gtk::ORIENTATION_VERTICAL),
      m_HPaned(Gtk::ORIENTATION_HORIZONTAL),
      m_LeftPane(),
      m_RightPane() {
  set_title("Map Editor");
  set_size_request(1200, 900);
  set_border_width(6);

  add(m_Box);

  // create actions
  m_refActionGroup = Gio::SimpleActionGroup::create();
  make_actions();

  m_refBuilder = Gtk::Builder::create();
  make_menu_bar();

  make_toolbars();

  m_HPaned.add1(m_LeftPane);
  m_HPaned.add2(m_RightPane);
  m_HPaned.set_position(1000);
  m_Box.pack_start(m_HPaned, Gtk::PACK_EXPAND_WIDGET);
  show_all_children();
}

MainWindow::~MainWindow() { std::cout << "destroying window" << std::endl; }

void MainWindow::make_actions() {
  m_refActionGroup->add_action("new",
                               sigc::mem_fun(*this, &MainWindow::on_menu_new));
  m_refActionGroup->add_action("open",
                               sigc::mem_fun(*this, &MainWindow::on_menu_open));
  m_refActionGroup->add_action("save",
                               sigc::mem_fun(*this, &MainWindow::on_menu_save));
  m_refActionGroup->add_action(
      "save_as", sigc::mem_fun(*this, &MainWindow::on_menu_save_as));
  m_refActionGroup->add_action("quit",
                               sigc::mem_fun(*this, &MainWindow::on_menu_quit));

  m_refActionGroup->add_action("help",
                               sigc::mem_fun(*this, &MainWindow::on_menu_help));
  m_refActionGroup->add_action(
      "about", sigc::mem_fun(*this, &MainWindow::on_menu_about));

  insert_action_group("menu_actions", m_refActionGroup);
}

void MainWindow::on_menu_new() {
  std::cout << "NEW action triggered." << std::endl;
}

void MainWindow::on_menu_open() {
  std::cout << "OPEN action triggered." << std::endl;
}

void MainWindow::on_menu_about() {
  std::cout << "ABOUT action triggered." << std::endl;
}

void MainWindow::on_menu_help() {
  std::cout << "HELP action triggered." << std::endl;
}

void MainWindow::on_menu_save() {
  std::cout << "SAVE action tirggered." << std::endl;
}

void MainWindow::on_menu_save_as() {
  std::cout << "SAVE AS action triggered." << std::endl;
}

void MainWindow::on_menu_quit() { hide(); }

void MainWindow::make_toolbars() {
  Gtk::Toolbar* toolbar = Gtk::manage(new Gtk::Toolbar());
  m_Box.pack_start(*toolbar, Gtk::PACK_SHRINK);
}

void MainWindow::make_menu_bar() {
  Glib::ustring ui_info =
      "<interface>"
      "  <menu id='top-menu'>"
      "    <submenu>"
      "      <attribute name='label'>_File</attribute>"
      "      <section>"
      "        <item>"
      "          <attribute name='label'>_New</attribute>"
      "          <attribute name='action'>menu_actions.new</attribute>"
      "          <attribute name='accel'>&lt;Primary&gt;n</attribute>"
      "        </item>"
      "        <item>"
      "          <attribute name='label'>_Open</attribute>"
      "          <attribute name='action'>menu_actions.open</attribute>"
      "          <attribute name='accel'>&lt;Primary&gt;o</attribute>"
      "        </item>"
      "      </section>"
      "      <section>"
      "        <item>"
      "          <attribute name='label'>_Save</attribute>"
      "          <attribute name='action'>menu_actions.save</attribute>"
      "          <attribute name='accel'>&lt;Primary&gt;s</attribute>"
      "        </item>"
      "        <item>"
      "          <attribute name='label'>Save as</attribute>"
      "          <attribute name='action'>menu_actions.save_as</attribute>"
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
      "      <attribute name='label'>_Help</attribute>"
      "      <section>"
      "        <item>"
      "          <attribute name='label'>_Help</attribute>"
      "          <attribute name='action'>menu_actions.help</attribute>"
      "          <attribute name='accel'>&lt;Primary&gt;h</attribute>"
      "        </item>"
      "      </section>"
      "      <section>"
      "        <item>"
      "          <attribute name='label'>_About</attribute>"
      "          <attribute name='action'>menu_actions.about</attribute>"
      "        </item>"
      "      </section>"
      "    </submenu>"
      "  </menu>"
      "</interface>";

  try {
    m_refBuilder->add_from_string(ui_info);
  } catch (const Glib::Error& ex) {
    std::cerr << "building menus failed: " << ex.what();
  }

  // get menubar
  Glib::RefPtr<Glib::Object> object = m_refBuilder->get_object("top-menu");
  Glib::RefPtr<Gio::Menu> gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
  if (!gmenu) g_warning("GMenu not found");

  // put it there
  Gtk::MenuBar* pMenubar = Gtk::manage(new Gtk::MenuBar(gmenu));
  m_Box.pack_start(*pMenubar, Gtk::PACK_SHRINK);
}
