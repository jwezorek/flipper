#pragma once

#include <array>
#include <unordered_set>
#include <unordered_map>

/*------------------------------------------------------------------------------------------------*/

namespace jw {

    namespace detail {
        size_t hash_combine(size_t seed, int v);
    }

    // 2D point
    struct point {
        float x;
        float y;
    };

    struct hex_grid_display_params {
        point origin; // screen location of the center hex{0,0}...
        float hex_radius;
        bool pointy_top;
    };

    // hex coordinates in the "axial" style discussed here:
    // https://www.redblobgames.com/grids/hexagon
    struct hex_coords {
        int q;
        int r;

        bool operator==(const hex_coords& rhs) const {
            return q == rhs.q && r == rhs.r;
        }
    };

    struct hash_hex {
        inline size_t operator()(const hex_coords& h) const {
            const size_t seed = detail::hash_combine(0, h.q);
            return detail::hash_combine(seed, h.r);
        }
    };

    using hex_set = std::unordered_set<hex_coords, hash_hex>;

    template<typename T>
    using hex_map = std::unordered_map<hex_coords, T, hash_hex>;

    using hex_grid = hex_map<int>;

    // returns the center of the hex in pixels...
    point hex_to_screen_loc(const hex_coords& hex, const hex_grid_display_params& params);
    hex_coords screen_loc_to_hex(const point& pt, const hex_grid_display_params& params);
    std::array<point, 6> hexagon_vertices(
        const hex_coords& hex, const hex_grid_display_params& params
    );
    std::array<hex_coords, 6> neighbors(const hex_coords& hex);

}
