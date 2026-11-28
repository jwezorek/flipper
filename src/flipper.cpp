#include "flipper.hpp"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

#include <stdexcept>

namespace {
    QJsonArray require_array(const QJsonValue& value, const char* name) {
        if (!value.isArray()) {
            throw std::runtime_error(std::string(name) + " must be an array");
        }
        return value.toArray();
    }

    int integer(const QJsonObject& cell, const char* key) {
        const auto value = cell.value(key);
        const int result = value.toInt();
        if (!value.isDouble() || value.toDouble() != result) {
            throw std::runtime_error(std::string(key) + " must be an integer in the int range");
        }
        return result;
    }

    int checked_color(int color) {
        if (color < 0 || color > jw::k_max_color_index) {
            throw std::runtime_error("Color index out of range: " + std::to_string(color));
        }
        return color;
    }

    jw::hex_grid read_grid(const QJsonValue& value, const char* name) {
        jw::hex_grid grid;
        for (const auto& entry : require_array(value, name)) {
            const auto cell = entry.toObject();
            const jw::hex_coords coords{integer(cell, "q"), integer(cell, "r")};
            if (!grid.emplace(coords, checked_color(integer(cell, "color"))).second) {
                throw std::runtime_error(std::string("Duplicate hex location in ") + name);
            }
        }
        return grid;
    }

    QJsonArray write_grid(const jw::hex_grid& grid) {
        QJsonArray cells;
        for (const auto& [coords, color] : grid) {
            cells.append(QJsonObject{{"q", coords.q}, {"r", coords.r},
                                     {"color", checked_color(color)}});
        }
        return cells;
    }

    std::runtime_error file_error(const QFileDevice& file) {
        return std::runtime_error((file.fileName() + ": " + file.errorString()).toStdString());
    }
}

std::vector<jw::flipper_level> jw::from_json(const std::string& path) {
    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::ReadOnly)) {
        throw file_error(file);
    }
    const auto bytes = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        throw file_error(file);
    }
    QJsonParseError error;
    const auto document = QJsonDocument::fromJson(bytes, &error);
    if (error.error != QJsonParseError::NoError) {
        throw std::runtime_error(path + " at byte " + std::to_string(error.offset) +
                                 ": " + error.errorString().toStdString());
    }
    if (!document.isArray()) {
        throw std::runtime_error("Expected an array of levels");
    }

    std::vector<flipper_level> levels;
    for (const auto& entry : document.array()) {
        const auto object = entry.toObject();
        flipper_level level{read_grid(object.value("initial_configuration"), "initial_configuration"),
                            read_grid(object.value("target"), "target"), {}};
        for (const auto& stamp : require_array(object.value("stamps"), "stamps")) {
            level.stamps.push_back(read_grid(stamp, "stamp"));
        }
        levels.push_back(std::move(level));
    }
    return levels;
}

void jw::to_json(const std::vector<flipper_level>& levels, const std::string& path) {
    QJsonArray entries;
    for (const auto& level : levels) {
        QJsonArray stamps;
        for (const auto& stamp : level.stamps) {
            stamps.append(write_grid(stamp));
        }
        entries.append(QJsonObject{{"initial_configuration", write_grid(level.initial_configuration)},
                                   {"target", write_grid(level.target)}, {"stamps", stamps}});
    }

    const auto bytes = QJsonDocument(entries).toJson();
    QSaveFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size() || !file.commit()) {
        throw file_error(file);
    }
}
