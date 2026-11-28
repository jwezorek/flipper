#include "game_board.hpp"
#include "main_window.hpp"
#include "hex_grid.hpp"
#include <qpainter.h>
#include <ranges>
#include <array>

namespace r = std::ranges;
namespace rv = std::ranges::views;

/*------------------------------------------------------------------------------------------------*/

namespace {

    
    const int k_board_sz = 4;
    const float k_cell_sz = 45.0f;
    const int k_stamp_columns = 2;
    const float k_marg_x = 45.0f;
    const float k_marg_y = 30.0f;
    const bool k_pointy_hexes = false;

    const auto k_palette = std::array<QColor, 3>{
        QColor::fromString("#D7191C"),
        QColor::fromString("#ABDDA4"),
        QColor::fromString("#2B83BA")
    };
    const auto k_bkgd_color = QColor::fromString("#FDAE61");

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
        k_pointy_hexes
    };

    bool is_on_hex_board(const jw::hex_coords& hex) {
        static auto board_cells = jw::hex_grid_cells(k_board_sz);
        return board_cells.contains(hex);
    }

    QPolygonF cell_polygon(const jw::hex_coords& hex,
            const jw::hex_grid_display_params& params) {
        return jw::hexagon_vertices(hex, params) | rv::transform(
            [](const auto& pt)->QPointF {
                return { pt.x, pt.y };
            }
        ) | r::to<QPolygonF>();
    }

    void paint_tiles(QPainter& p, const jw::hex_grid& tiles,
            const jw::hex_grid_display_params& params) {

        static std::vector<QBrush> palette;
        if (palette.empty()) {
            palette = k_palette | rv::transform(
                [](const auto& color)->QBrush {
                    return QBrush(color);
                }
            ) | r::to<std::vector>();
        }

        for (const auto& [loc, color] : tiles) {
            if (color < 0 || color >= palette.size()) {
                continue;
            }
            p.setBrush(palette[color]);
            p.drawPolygon(cell_polygon(loc, params));
        }
    }

    void paint_hex_grid(QPainter& p, const jw::hex_grid& tiles, int sz, 
            const jw::hex_grid_display_params& params) {

        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(Qt::white);
        p.setPen(QPen(Qt::black, 1.0));

        auto cells = jw::hex_grid_cells(sz);
        for (const auto& hex : cells) {
            p.drawPolygon(cell_polygon(hex, params));
        }

        paint_tiles(p, tiles, params);
    }

    void draw_stamps(QPainter& p, const std::vector<jw::hex_grid>& stamps) {

        auto n = static_cast<int>(stamps.size());
        int rows = n / k_stamp_columns + (n % k_stamp_columns != 0);
        auto left = jw::k_board_hgt + k_marg_x;
        auto top = k_marg_y;
        auto col_width = (jw::k_board_wd - left) / k_stamp_columns;
        auto row_height = (jw::k_board_hgt - 2 * k_marg_y) / rows;
        auto offset_x = col_width / 2.0f;
        auto offset_y = row_height / 2.0f;

        for (int i = 0; i < n; ++i) {
            auto col = i % k_stamp_columns;
            auto row = i / k_stamp_columns;
            auto x = left + (col * col_width) + offset_x;
            auto y = top + (row * row_height) + offset_y;

            auto stamp_params = jw::hex_grid_display_params{
                jw::point{ x, y },
                30.0,
                k_pointy_hexes
            };

            paint_hex_grid(p, stamps.at(i), 1, stamp_params);
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
    
    if (!state_) {
        return;
    }

    paint_hex_grid(p, state_->state.current_tiles_, k_board_sz, k_params);
    draw_stamps(p, state_->lvl_def.stamps);

    if (cursor_cell_) {
        p.setBrush(Qt::gray);
        p.setPen(QPen(Qt::black, 1.0));
        auto poly = cell_polygon(*cursor_cell_, k_params);
        p.drawPolygon(poly);
    }
}

void jw::game_board::set(const flipper_level& lvl) {

    state_ = level_state{
        lvl,
        game_state {
            lvl.initial_configuration,
            std::nullopt
        }
    };
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


