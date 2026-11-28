#pragma once

#include <QWidget>
#include <QElapsedTimer>
#include <QTimer>
#include <optional>
#include "flipper.hpp"

/*------------------------------------------------------------------------------------------------*/

namespace jw {

    class game_board : public QWidget {

        Q_OBJECT

        std::optional<level_state> state_;
        std::optional<hex_coords> cursor_cell_;

        QTimer timer_;
        QElapsedTimer elapsed_;

    public:
        explicit game_board(QWidget* parent = nullptr);
        void paintEvent(QPaintEvent*) override;
        void set(const flipper_level& lvl);
        void tick();
    };


}