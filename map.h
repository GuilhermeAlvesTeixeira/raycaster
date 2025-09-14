//
// Created by usuario on 14
#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <string>

class Map {
public:
    Map(float cellSize, int width, int height);
    //Map(float cellSize, const std::vector<std::vector<int>> &grid);
    Map(float cellSize, const std::string &filename);

    void draw(sf::RenderTarget& window);

    const std::vector<std::vector<sf::Color>>& getGrid() const;
    float getCellSize() const;

private:
    std::vector<std::vector<sf::Color>> grid;
    float cellSize;
};

#endif //MAP_H
