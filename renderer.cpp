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
#include <SFML/Graphics/RectangleShape.hpp>

#include <cmath>
#include <iostream>
#include <limits>


constexpr float PI                      = 3.14159265359f;
constexpr size_t MAX_RAYCAST_DEPTH      = 16;                               //Alcance máximo do raio sem bater em nada
constexpr size_t MAX_RAYCASTING_STEPS   = 64;                               //Aqui o raio não vai atravessar mais que 64 células no grid
constexpr float PLAYER_FOV              = 60.0f;                            //Campo de visão do jogador
constexpr size_t NUM_RAYS               = 120;                              //Número de raios lançados
constexpr float COLUMN_WIDTH            = SCREEN_W / (float) NUM_RAYS;

struct Ray {
    sf::Vector2f hitPosition;
    float distance;
    bool hit;
};

static Ray castRay(sf::Vector2f start, float angleInDegrees, const Map &map);

void Renderer::draw3dView(sf::RenderTarget &target, const Player &player, const Map &map) {

    float angle = player.angle - PLAYER_FOV / 2.0f;
    float angleIncrement = PLAYER_FOV / (float) NUM_RAYS;

    for (size_t i = 0; i < NUM_RAYS; i++, angle += angleIncrement) {
        Ray ray = castRay(player.position, angle, map);

        if (ray.hit) {
            float wallHeight = (map.getCellSize() * SCREEN_H) / ray.distance;

            if (wallHeight > SCREEN_H) {
                wallHeight = SCREEN_H;
            }

            float wallOffset = SCREEN_H / 2.0f - wallHeight / 2.0f;

            sf::RectangleShape column(sf::Vector2f(COLUMN_WIDTH, wallHeight));
            column.setPosition(i * COLUMN_WIDTH,  wallOffset);
            target.draw(column);
        }
    }
}

void Renderer::drawRays(sf::RenderTarget &target, const Player &player, const Map &map) {

    for (float angle = player.angle - PLAYER_FOV/2.0f; angle < player.angle + PLAYER_FOV; angle += 1.0f) {
        Ray ray = castRay(player.position, angle, map);

        if (ray.hit) {
            sf::Vertex line[] = {
                sf::Vertex(player.position),
                sf::Vertex(ray.hitPosition),
            };
            line[0].color = sf::Color::Red;
            line[1].color = sf::Color::Red;

            target.draw(line, 2, sf::Lines);
        }
    }
}

Ray castRay(sf::Vector2f start, float angleInDegrees, const Map &map) {
    float angle = angleInDegrees * PI / 180.f;
    float vTan = -std::tan(angle), hTan = -(1.0f) / std::tan(angle);
    float cellSize = map.getCellSize();

    bool hit = false;

    size_t vdof = 0, hdof = 0;                                                               //controladores de profundidade. numero de passos até colidir na vertical ou horizontal
    float vdist = std::numeric_limits<float>::max();
    float hdist = std::numeric_limits<float>::max();

    // *********************** Calculando interseções verticais ************************
    sf::Vector2f vRayPos, hRayPos, offset;
    if (std::cos(angle) > 0.001f) {                                                          //O jogador está olhando para baixo no sistema de coordenadas de tela
        vRayPos.x = std::floor(start.x / cellSize) * cellSize + cellSize;
        vRayPos.y = (start.x - vRayPos.x) * vTan + start.y;                                  //eq. da reta dado y conhecido

        offset.x = cellSize;
        offset.y = -offset.x * vTan;
        //std::cout << "Olhando para esquerda" << '\n';

    } else if (std::cos(angle) < -0.001f) {                                                 //O jogador está olhando para cima no sistema de coordenadas de tela
        vRayPos.x = std::floor(start.x / cellSize) * cellSize - 0.01f;
        vRayPos.y = (start.x - vRayPos.x) * vTan + start.y;                                 //eq. da reta dado y conhecido

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
            hit = true;
            //Calcula a distancia vertical entre a posição do jogador e a posição do raio
            vdist = std::sqrt(
            (vRayPos.x - start.x) * (vRayPos.x - start.x)
            + (vRayPos.y - start.y) * (vRayPos.y - start.y)
            );
            break;
        }

        vRayPos += offset;
    }

    // *********************** Calculando interseções horizontais ************************

    if (std::sin(angle) > 0.001f) {                                                          //O jogador está olhando para baixo no sistema de coordenadas de tela
        hRayPos.y = std::floor(start.y / cellSize) * cellSize + cellSize;
        hRayPos.x = (start.y - hRayPos.y) * hTan + start.x;                                  //eq. da reta dado y conhecido
        offset.y = cellSize;
        offset.x = -offset.y * hTan;
        //std::cout << "Olhando para baixo" << '\n';

    } else if (std::sin(angle) < -0.001f) {                                                 //O jogador está olhando para cima no sistema de coordenadas de tela
        hRayPos.y = std::floor(start.y/ cellSize) * cellSize - 0.01f;
        hRayPos.x = (start.y - hRayPos.y) * hTan + start.x;                                  // eq.da reta dado y conhecido
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
            hit = true;
            //Calcula a distancia horizontal entre a posição do jogador e a posição do raio
            hdist = std::sqrt(
            (hRayPos.x - start.x) * (hRayPos.x - start.x)
            + (hRayPos.y - start.y) * (hRayPos.y - start.y)
            );
            break;
        }

        hRayPos += offset;
    }
    return Ray {hdist < vdist ? hRayPos : vRayPos, std::min(hdist,vdist), hit};
}
