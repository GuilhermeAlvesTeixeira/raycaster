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

constexpr float PI                      = 3.14159265359f;
constexpr size_t MAX_RAYCAST_DEPTH      = 64;                               //Alcance máximo do raio sem bater em nada
constexpr size_t MAX_RAYCASTING_STEPS   = 64;                               //Aqui o raio não vai atravessar mais que 64 células no grid
constexpr float PLAYER_FOV              = 60.0f;                            //Campo de visão do jogador

/* talvez não seja mais necessário
struct Ray {
    sf::Vector2f hitPosition;
    sf::Vector2u mapPosition;
    float distance;
    bool hit;
    bool isHitVertical;
};*/

void Renderer::init() {
    if (!wallTexture.loadFromFile("../Textures/hell/wall2.bmp")) {
        std::cerr << "Erro ao carregar textura" << '\n';
        return;
    };

    if (wallTexture.getSize().x != wallTexture.getSize().y) {
        std::cerr << "ERRO: Textura não possui as mesmas dimensões " << '\n';
        return;
    }

    wallSprite = sf::Sprite(wallTexture);
}

void Renderer::draw3dView(sf::RenderTarget &target, const Player &player, const Map &map) {

    //Céu
    sf::RectangleShape rectangle(sf::Vector2f(SCREEN_W, SCREEN_H /2.0f));
    rectangle.setFillColor(sf::Color(80,30,30));
    target.draw(rectangle);

    //Chão
    rectangle.setPosition(0.0f, SCREEN_H /2.0f);
    rectangle.setFillColor(sf::Color(180,0,0));
    target.draw(rectangle);

    const float maxRenderDistance = MAX_RAYCAST_DEPTH * map.getCellSize();
    const float maxFogDistance = maxRenderDistance / 4.0f;

    float radians = player.angle * PI/180.0f;
    sf::Vector2f direction{std::cos(radians), std::sin(radians)};
    sf::Vector2f plane {-direction.y, direction.x};

    sf::VertexArray walls{sf::Lines};

    for (size_t i = 0; i < SCREEN_W; i++) {
        float cameraX = i * 2.0f / SCREEN_W - 1.0f; // -1.0f -> 0.0f -> 1.0f
        sf::Vector2f rayPos = player.position / map.getCellSize();
        sf::Vector2f rayDir = direction + plane * cameraX;

        sf::Vector2f deltaDist {
            std::abs(1.0f / rayDir.x),
            std::abs(1.0f / rayDir.y)
        };

        sf::Vector2i mapPos{rayPos};
        sf::Vector2i step;
        sf::Vector2f sideDist;

        if (rayDir.x < 0.0f) {  //direita
            step.x = -1;
            sideDist.x = (-mapPos.x + rayPos.x) * deltaDist.x;
        } else {               // esquerda
            step.x = 1;
            sideDist.x = (mapPos.x - rayPos.x + 1.0f) * deltaDist.x;
        }

        if (rayDir.y < 0.0f) {  //direita
            step.y = -1;
            sideDist.y = (-mapPos.y + rayPos.y) * deltaDist.y;
        } else {               // esquerda
            step.y = 1;
            sideDist.y = (mapPos.y - rayPos.y + 1.0f) * deltaDist.y;
        }

        // ********************** Hora do dda *******************************

        bool didHit {}, isHitVertical {};
        size_t depth = 0;

        while (!didHit && depth < MAX_RAYCAST_DEPTH) {
            if (sideDist.x < sideDist.y) {
                sideDist.x += deltaDist.x;
                mapPos.x += step.x;
                isHitVertical = false;
            } else {
                sideDist.y += deltaDist.y;
                mapPos.y += step.y;
                isHitVertical = true;
            }

            int x = mapPos.x, y = mapPos.y;
            const auto &grid = map.getGrid();

            if (y >= 0 && y <  grid.size() && x >= 0 && x < grid[y].size() && grid[y][x] != sf::Color::Black) {
                didHit = true;
            }
            depth++;
        }
        float perpWallDist = isHitVertical? sideDist.y - deltaDist.y : sideDist.x - deltaDist.x;
        float wallHeight = SCREEN_H / perpWallDist;
        float wallStart = (-wallHeight + SCREEN_H)/2.0f;
        float wallEnd = (wallHeight + SCREEN_H)/2.0f;

        walls.append(sf::Vertex(sf::Vector2f((float)i, wallStart)));
        walls.append(sf::Vertex(sf::Vector2f((float)i, wallEnd)));
    }
    target.draw(walls);
}


