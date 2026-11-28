#pragma once

#include <string>
#include <vector>
#include <optional>
#include "hex_grid.hpp"

namespace jw {

    constexpr int k_max_color_index = 2;

    struct flipper_level {
        hex_grid initial_configuration;
        hex_grid target;
        std::vector<hex_grid> stamps;
    };

    struct game_state {
        hex_grid current_tiles_;
        std::optional<int> current_stamp_;
    };

    struct level_state {
        flipper_level lvl_def;
        game_state state;
    };

    std::vector<flipper_level> from_json(const std::string& path);
    void to_json(const std::vector<flipper_level>& levels, const std::string& path);

}
