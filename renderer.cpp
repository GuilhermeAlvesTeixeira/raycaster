//
// Created by usuario on 14/09/25.
//
/*
 * O raio é uma reta
 * y = m(x - x0) + y0, onde m = tan(angle)
 *
 */
#include "renderer.h"
#include <cmath>
#include <iostream>

constexpr float PI = 3.14159265359f;
constexpr size_t MAX_RAYCASTING_STEPS = 64;

void Renderer::drawRays(sf::RenderTarget &target, const Player &player, const Map &map) {

    float angle = player.angle * PI / 180.f;
    float aTan = -(1.0f) / tan(angle);
    float cellSize = map.getCellSize();

    sf::Vector2f rayPos, offset;

    //Calculando interseções horizontais
    if (sin(angle) > 0.001f) {              //O jogador está olhando para baixo
        rayPos.y = std::round(player.position.y / cellSize) * cellSize + cellSize;
        rayPos.x = (player.position.y - rayPos.y) * aTan + player.position.x;           //eq. da reta
        offset.y = cellSize;
        offset.x = -offset.y * aTan;
        std::cout << "Olhando para baixo" << '\n';

    } else if (sin(angle) < -0.001f) {      //O jogador está olhando para cima
        rayPos.y = std::round(player.position.y / cellSize) * cellSize;
        rayPos.x = (player.position.y - rayPos.y) * aTan + player.position.x;          // eq.da reta
        offset.y = -cellSize;
        offset.x = -offset.y * aTan;
        std::cout << "Olhando para cima" << '\n';
    } else {
        return;
    }

    //******************** Digital Differential Analysis (DDA) ***********************

    const auto &grid = map.getGrid();
    for (size_t i = 0; i < MAX_RAYCASTING_STEPS; i++) {
        int mapX = (int)(rayPos.x / cellSize);
        int mapY = (int)(rayPos.y / cellSize);

        if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX]) {
            break;
        }

        rayPos += offset;
    }

    sf::Vertex line[] = {
        sf::Vertex(player.position),
        sf::Vertex(rayPos),
    };

    target.draw(line,2,sf::Lines);
}