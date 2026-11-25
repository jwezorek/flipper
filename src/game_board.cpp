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

    void draw_hex_grid(QPainter& p, int sz) {

        auto half_hgt = static_cast<float>(jw::k_board_hgt) / 2.0f;
        auto origin = jw::point{ half_hgt,half_hgt };

        jw::hex_grid_display_params params{
            origin,
            45.0f,
            true
        };

        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(Qt::white);
        p.setPen(QPen(Qt::black, 1.0));

        auto cells = jw::hex_grid_cells(sz);
        for (const auto& hex : cells) {
            p.drawPolygon(
                jw::hexagon_vertices(hex, params) | rv::transform(
                    [](const auto& pt)->QPointF {
                        return { pt.x, pt.y };
                    }
                ) | r::to<QPolygonF>()
            );
        }
    }
}

jw::game_board::game_board(QWidget* parent) : QWidget(parent)
{}

void jw::game_board::paintEvent(QPaintEvent*) {

    QPainter p(this);
    p.fillRect(rect(), k_bkgd_color);
    p.setRenderHint(QPainter::Antialiasing);

    draw_hex_grid(p, k_board_sz);

}

void jw::game_board::set(const flipper_level& lvl) {

    current_level_ = lvl;
    update();

}


