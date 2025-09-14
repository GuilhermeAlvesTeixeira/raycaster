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
    float vTan = -std::tan(angle);
    float hTan = -(1.0f) / std::tan(angle);
    float cellSize = map.getCellSize();

    // *********************** Calculando interseções verticais ************************
    sf::Vector2f rayPos, offset;
    if (std::cos(angle) > 0.001f) {                                                          //O jogador está olhando para baixo no sistema de coordenadas de tela
        rayPos.x = std::floor(player.position.x / cellSize) * cellSize + cellSize;
        rayPos.y = (player.position.x - rayPos.x) * vTan + player.position.y;                //eq. da reta dado y conhecido

        offset.x = cellSize;
        offset.y = -offset.x * vTan;
        //std::cout << "Olhando para esquerda" << '\n';

    } else if (std::cos(angle) < -0.001f) {                                                 //O jogador está olhando para cima no sistema de coordenadas de tela
        rayPos.x = std::floor(player.position.x / cellSize) * cellSize;
        rayPos.y = (player.position.x - rayPos.x) * vTan + player.position.y;               //eq. da reta dado y conhecido

        offset.x = -cellSize;
        offset.y = -offset.x * vTan;
        //std::cout << "Olhando para direita" << '\n';
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

    sf::Vertex lineV[] = {
        sf::Vertex(player.position),
        sf::Vertex(rayPos),
    };
    lineV[0].color = sf::Color::Yellow;
    lineV[1].color = sf::Color::Yellow;

    target.draw(lineV,2,sf::Lines);

    // *********************** Calculando interseções horizontais ************************

    if (std::sin(angle) > 0.001f) {                                                          //O jogador está olhando para baixo no sistema de coordenadas de tela
        rayPos.y = std::floor(player.position.y / cellSize) * cellSize + cellSize;
        rayPos.x = (player.position.y - rayPos.y) * hTan + player.position.x;           //eq. da reta dado y conhecido
        offset.y = cellSize;
        offset.x = -offset.y * hTan;
        //std::cout << "Olhando para baixo" << '\n';

    } else if (std::sin(angle) < -0.001f) {                                                 //O jogador está olhando para cima no sistema de coordenadas de tela
        rayPos.y = std::floor(player.position.y / cellSize) * cellSize;
        rayPos.x = (player.position.y - rayPos.y) * hTan + player.position.x;          // eq.da reta dado y conhecido
        offset.y = -cellSize;
        offset.x = -offset.y * hTan;
        //std::cout << "Olhando para cima" << '\n';
    } else {
        return;
    }

    //******************** Digital Differential Analysis (DDA) ***********************

    for (size_t i = 0; i < MAX_RAYCASTING_STEPS; i++) {
        int mapX = (int)(rayPos.x / cellSize);
        int mapY = (int)(rayPos.y / cellSize);

        if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX]) {
            break;
        }

        rayPos += offset;
    }

    sf::Vertex lineH[] = {
        sf::Vertex(player.position),
        sf::Vertex(rayPos),
    };
    lineH[0].color = sf::Color::Red;
    lineH[1].color = sf::Color::Red;

    target.draw(lineH, 2, sf::Lines);
}
