#pragma once

#include <QMainWindow>
#include <QString>
#include "game_board.hpp"

class QAction;

/*------------------------------------------------------------------------------------------------*/

namespace jw {

    constexpr int k_board_wd = 1280;
    constexpr int k_board_hgt = 720;

    class main_window : public QMainWindow {
        Q_OBJECT

    public:
        explicit main_window(QWidget* parent = nullptr);


    private:

        game_board* board_;
    };

} 