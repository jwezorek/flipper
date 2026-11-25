#include "main_window.hpp"

/*------------------------------------------------------------------------------------------------*/

jw::main_window::main_window(QWidget* parent) : QMainWindow(parent) {
    setCentralWidget(board_ = new game_board(this));
    setWindowTitle(tr("flipper"));
    board_->setFixedSize({ k_board_wd, k_board_hgt });
}

