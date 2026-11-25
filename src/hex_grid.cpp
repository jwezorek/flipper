#include "hex_grid.hpp"

#include <cmath>
#include <functional>
#include <numbers>
#include <ranges>

namespace r = std::ranges;
namespace rv = std::ranges::views;

/*------------------------------------------------------------------------------------------------*/

namespace {
    constexpr double sqrt3 = std::numbers::sqrt3;
}

size_t jw::detail::hash_combine(size_t seed, int v) {
    return seed ^ (std::hash<int>{}(v) + 0x9e3779b9u + (seed << 6) + (seed >> 2));
}

jw::point jw::hex_to_screen_loc(const hex_coords& hex, const hex_grid_display_params& params) {
    const double q = hex.q;
    const double r = hex.r;
    const double x = params.pointy_top ? sqrt3 * (q + r / 2) : 1.5 * q;
    const double y = params.pointy_top ? 1.5 * r : sqrt3 * (r + q / 2);
    return {
        static_cast<float>(params.origin.x + params.hex_radius * x),
        static_cast<float>(params.origin.y + params.hex_radius * y)
    };
}

jw::hex_coords jw::screen_loc_to_hex(const point& pt, const hex_grid_display_params& params) {
    const double x = (static_cast<double>(pt.x) - params.origin.x) / params.hex_radius;
    const double y = (static_cast<double>(pt.y) - params.origin.y) / params.hex_radius;
    const double q = params.pointy_top ? x / sqrt3 - y / 3 : 2 * x / 3;
    const double r = params.pointy_top ? 2 * y / 3 : y / sqrt3 - x / 3;
    const double s = -q - r;

    // Restore q + r + s = 0 by correcting the component with the largest rounding error.
    double rounded_q = std::round(q);
    double rounded_r = std::round(r);
    const double rounded_s = std::round(s);
    const double q_error = std::abs(rounded_q - q);
    const double r_error = std::abs(rounded_r - r);
    const double s_error = std::abs(rounded_s - s);
    if (q_error > r_error && q_error > s_error) {
        rounded_q = -rounded_r - rounded_s;
    } else if (r_error > s_error) {
        rounded_r = -rounded_q - rounded_s;
    }
    return {static_cast<int>(rounded_q), static_cast<int>(rounded_r)};
}

std::array<jw::point, 6> jw::hexagon_vertices(const hex_coords& hex,
        const hex_grid_display_params& params) {
    const point center = hex_to_screen_loc(hex, params);
    const float radius = params.hex_radius;
    const float half = radius / 2;
    const float apothem = static_cast<float>(radius * sqrt3 / 2);
    std::array<point, 6> vertices{{
        {radius, 0}, {half, apothem}, {-half, apothem},
        {-radius, 0}, {-half, -apothem}, {half, -apothem}
    }};
    for (auto& vertex : vertices) {
        if (params.pointy_top) {
            vertex = {vertex.y, -vertex.x};
        }
        vertex.x += center.x;
        vertex.y += center.y;
    }
    return vertices;
}

std::array<jw::hex_coords, 6> jw::neighbors(const hex_coords& hex) {
    const auto [q, r] = hex;
    return {{{q + 1, r}, {q, r + 1}, {q - 1, r + 1},
             {q - 1, r}, {q, r - 1}, {q + 1, r - 1}}};
}

jw::hex_set jw::hex_grid_cells(int n) {
    return rv::cartesian_product(
        rv::iota(-n, n + 1),
        rv::iota(-n, n + 1),
        rv::iota(-n, n + 1)
    ) | rv::filter(
        [](auto triple)->bool {
            auto [q, r, s] = triple;
            return q + r + s == 0;
        }
    ) | rv::transform(
        [](auto triple)->hex_coords {
            auto [q, r, s] = triple;
            return { q,r };
        }
    ) | r::to<hex_set>();
}
