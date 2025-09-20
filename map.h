//
// Created by usuario on 14
#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <filesystem>
#include <vector>
#include <string>

typedef std::vector<std::vector<int>> MapGrid;

class Map {
public:
    Map(float cellSize);
    Map(float cellSize, int width, int height);
    Map(float cellSize, const std::string &filename);

    void draw(sf::RenderTarget& window);
    void setMapCell(int x, int y, int value);

    void save(const std::filesystem::path &path);
    void load(const std::filesystem::path &path);

    const MapGrid & getGrid() const;
    float getCellSize() const;

private:
    MapGrid grid;
    float cellSize;
};

#endif //MAP_H
