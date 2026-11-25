#include "main_window.hpp"

/*------------------------------------------------------------------------------------------------*/

jw::main_window::main_window(QWidget* parent) : QMainWindow(parent) {
    setCentralWidget(board_ = new QWidget(this));
    setWindowTitle(tr("flipper"));
    resize(1200, 800);
}

