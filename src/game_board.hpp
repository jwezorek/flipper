#pragma once

#include <QWidget>
#include <optional>
#include <variant>
#include "flipper.hpp"

/*------------------------------------------------------------------------------------------------*/

namespace jw {

    using hover_state = std::variant<hex_coords, int>;

    class game_board : public QWidget {

        Q_OBJECT

        std::optional<level_state> state_;
        std::optional<hover_state> hover_item_;

    protected:
        void mouseMoveEvent(QMouseEvent* event) override;
        void leaveEvent(QEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;

    public:
        explicit game_board(QWidget* parent = nullptr);
        void paintEvent(QPaintEvent*) override;
        void set(const flipper_level& lvl);
    };


}
