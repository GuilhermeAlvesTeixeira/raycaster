//
// Created by usuario on 14/09/25.
//
/*
 * O raio é uma reta
 * y = m(x - x0) + y0, onde m = tan(angle)
 *
 */
#include "renderer.h">
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <iostream>
#include <limits>

constexpr float PI = 3.14159265359f;
constexpr size_t MAX_RAYCAST_DEPTH = 16;
constexpr size_t MAX_RAYCASTING_STEPS = 64;

void Renderer::drawRays(sf::RenderTarget &target, const Player &player, const Map &map) {

    float angle = player.angle * PI / 180.f;
    float vTan = -std::tan(angle), hTan = -(1.0f) / std::tan(angle);
    float cellSize = map.getCellSize();

    size_t vdof = 0, hdof = 0;
    float vdist = std::numeric_limits<float>::max();
    float hdist = std::numeric_limits<float>::max();

    // *********************** Calculando interseções verticais ************************
    sf::Vector2f vRayPos, hRayPos, offset;
    if (std::cos(angle) > 0.001f) {                                                          //O jogador está olhando para baixo no sistema de coordenadas de tela
        vRayPos.x = std::floor(player.position.x / cellSize) * cellSize + cellSize;
        vRayPos.y = (player.position.x - vRayPos.x) * vTan + player.position.y;                //eq. da reta dado y conhecido

        offset.x = cellSize;
        offset.y = -offset.x * vTan;
        //std::cout << "Olhando para esquerda" << '\n';

    } else if (std::cos(angle) < -0.001f) {                                                 //O jogador está olhando para cima no sistema de coordenadas de tela
        vRayPos.x = std::floor(player.position.x / cellSize) * cellSize - 0.01f;
        vRayPos.y = (player.position.x - vRayPos.x) * vTan + player.position.y;               //eq. da reta dado y conhecido

        offset.x = -cellSize;
        offset.y = -offset.x * vTan;
        //std::cout << "Olhando para direita" << '\n';
    } else {
        vdof = MAX_RAYCAST_DEPTH;
    }

    //******************** Digital Differential Analysis (DDA) na vertical ***********************

    const auto &grid = map.getGrid();
    for (; vdof < MAX_RAYCASTING_STEPS; vdof++) {
        int mapX = (int)(vRayPos.x / cellSize);
        int mapY = (int)(vRayPos.y / cellSize);

        if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX]) {

            //Calcula a distancia vertical entre a posição do jogador e a posição do raio
            vdist = std::sqrt(
            (vRayPos.x - player.position.x) * (vRayPos.x - player.position.x)
            + (vRayPos.y - player.position.y) * (vRayPos.y - player.position.y)
            );
            break;
        }

        vRayPos += offset;
    }

    // *********************** Calculando interseções horizontais ************************

    if (std::sin(angle) > 0.001f) {                                                          //O jogador está olhando para baixo no sistema de coordenadas de tela
        hRayPos.y = std::floor(player.position.y / cellSize) * cellSize + cellSize;
        hRayPos.x = (player.position.y - hRayPos.y) * hTan + player.position.x;           //eq. da reta dado y conhecido
        offset.y = cellSize;
        offset.x = -offset.y * hTan;
        //std::cout << "Olhando para baixo" << '\n';

    } else if (std::sin(angle) < -0.001f) {                                                 //O jogador está olhando para cima no sistema de coordenadas de tela
        hRayPos.y = std::floor(player.position.y / cellSize) * cellSize - 0.01f;
        hRayPos.x = (player.position.y - hRayPos.y) * hTan + player.position.x;          // eq.da reta dado y conhecido
        offset.y = -cellSize;
        offset.x = -offset.y * hTan;
        //std::cout << "Olhando para cima" << '\n';
    } else {
        hdof = MAX_RAYCAST_DEPTH;
    }

    //******************** Digital Differential Analysis (DDA) na Horizontal ***********************

    for (; hdof < MAX_RAYCASTING_STEPS; hdof++) {
        int mapX = (int)(hRayPos.x / cellSize);
        int mapY = (int)(hRayPos.y / cellSize);

        if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX]) {

            //Calcula a distancia horizontal entre a posição do jogador e a posição do raio
            hdist = std::sqrt(
            (hRayPos.x - player.position.x) * (hRayPos.x - player.position.x)
            + (hRayPos.y - player.position.y) * (hRayPos.y - player.position.y)
            );
            break;
        }

        hRayPos += offset;
    }

    sf::Vertex line[] = {
        sf::Vertex(player.position),
        sf::Vertex(hdist < vdist ? hRayPos : vRayPos),
    };
    line[0].color = sf::Color::Red;
    line[1].color = sf::Color::Red;

    target.draw(line, 2, sf::Lines);

}
