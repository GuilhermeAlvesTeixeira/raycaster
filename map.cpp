//
// Created by usuario on 14/09/25.
//
#include "map.h"

#include <iostream>
#include <ostream>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

//Construtores da classe
Map::Map(float cellSize, int width, int height)
    : cellSize(cellSize), grid(height, std::vector(width,0)) {}

Map::Map(float cellSize, const std::string &filename) : cellSize(cellSize) {
    sf::Image image;
    if (!image.loadFromFile(filename)){
        std::cerr << "Erro ao carregar o mapa da imagem:  " << filename << '\n';
        return;
    }

    grid = std::vector(image.getSize().y,
        std::vector(image.getSize().x, 0));

    for (size_t y = 0; y < image.getSize().y; y++) {
        for (size_t x = 0; x < image.getSize().x; x++) {
            grid[y][x] = image.getPixel(x,y) == sf::Color::Black? 0 : 1;
        }
    }
}

//Metodo draw()
void Map::draw(sf::RenderTarget &target ) {

    if (grid.empty()) {
        return;
    }
    sf::RectangleShape background(sf::Vector2f(
        (float)grid[0].size() * cellSize,
        (float)grid.size() * cellSize));

    background.setFillColor(sf::Color::Black);
    target.draw(background);

    sf::RectangleShape cell(sf::Vector2f(cellSize * 0.95f, cellSize * 0.95f));

    for (size_t y = 0; y < grid.size(); y++) {
        for (size_t x = 0; x < grid[y].size(); x++) {
            cell.setFillColor(grid[y][x] ? sf::Color::White : sf::Color(70,70,70));

            cell.setPosition(sf::Vector2f(x,y) * cellSize +
                sf::Vector2f(cellSize * 0.025f , cellSize * 0.025f));
            target.draw(cell);
        }
    }
}

const MapGrid &Map::getGrid() const { return grid; }

float Map::getCellSize() const { return cellSize; }