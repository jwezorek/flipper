#pragma once

#include <string>
#include <vector>
#include <optional>
#include "hex_grid.hpp"

namespace jw {

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
    void to_json(const std::vector<flipper_level>& level);

}