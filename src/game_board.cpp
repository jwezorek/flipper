#include "game_board.hpp"
#include "main_window.hpp"
#include "hex_grid.hpp"
#include <qpainter.h>
#include <ranges>

namespace r = std::ranges;
namespace rv = std::ranges::views;

/*------------------------------------------------------------------------------------------------*/

namespace {

    const auto k_bkgd_color = QColor(172, 120, 186);
    const int k_board_sz = 4;
    const float k_cell_sz = 45.0f;

    jw::point from_qt_point(const QPoint& p) {
        return {
            static_cast<float>(p.x()),
            static_cast<float>(p.y())
        };
    }

    jw::hex_grid_display_params k_params{
        jw::point{ static_cast<float>(jw::k_board_hgt) / 2.0f,
            static_cast<float>(jw::k_board_hgt) / 2.0f 
        },
        k_cell_sz,
        false
    };

    bool is_on_hex_board(const jw::hex_coords& hex) {
        static auto board_cells = jw::hex_grid_cells(k_board_sz);
        return board_cells.contains(hex);
    }

    QPolygonF cell_polygon(const jw::hex_coords& hex,
            const jw::hex_grid_display_params& params) {
        return jw::hexagon_vertices(hex, k_params) | rv::transform(
            [](const auto& pt)->QPointF {
                return { pt.x, pt.y };
            }
        ) | r::to<QPolygonF>();
    }

    void draw_hex_grid(QPainter& p, int sz) {

        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(Qt::white);
        p.setPen(QPen(Qt::black, 1.0));

        auto cells = jw::hex_grid_cells(sz);
        for (const auto& hex : cells) {
            p.drawPolygon(cell_polygon(hex, k_params));
        }
    }
}

jw::game_board::game_board(QWidget* parent) : QWidget(parent) {

    connect(&timer_, &QTimer::timeout, this, &game_board::tick);
    timer_.setTimerType(Qt::PreciseTimer);
    elapsed_.start();
    timer_.start(16);  // Request a tick about every 16 ms.

}

void jw::game_board::paintEvent(QPaintEvent*) {

    QPainter p(this);
    p.fillRect(rect(), k_bkgd_color);
    p.setRenderHint(QPainter::Antialiasing);

    draw_hex_grid(p, k_board_sz);

    if (cursor_cell_) {
        p.setBrush(Qt::gray);
        p.setPen(QPen(Qt::black, 1.0));
        auto poly = cell_polygon(*cursor_cell_, k_params);
        p.drawPolygon(poly);
    }
}

void jw::game_board::set(const flipper_level& lvl) {

    current_level_ = lvl;
    update();

}

void jw::game_board::tick() {

    const double dt = elapsed_.restart() / 1000.0;  // Seconds since last tick.
    const QPoint cursor = mapFromGlobal(QCursor::pos());

    auto current_cell = jw::screen_loc_to_hex(
        from_qt_point(cursor), k_params
    );

    if (is_on_hex_board(current_cell)) {
        cursor_cell_ = current_cell;
    } else {
        cursor_cell_ = {};
    }

    update();
}


