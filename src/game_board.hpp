#pragma once

#include <QWidget>
#include <optional>
#include "flipper.hpp"

/*------------------------------------------------------------------------------------------------*/

namespace jw {

    class game_board : public QWidget {

        Q_OBJECT

        std::optional<flipper_level> current_level_;

    public:
        explicit game_board(QWidget* parent = nullptr);
        void paintEvent(QPaintEvent*) override;
        void set(const flipper_level& lvl);
    };


}