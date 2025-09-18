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
constexpr float PLAYER_FOV              = 60.0f;                            //Campo de visão do jogador
constexpr float CAMERA_Z                = 0.5 * SCREEN_H;                   //No centro da tela

const std::string WALL_PATH  = "../Textures/Lab/[SUB]wall_A_003.png";         //Walls
const std::string FLOOR_PATH = "../Textures/Lab/[SUB]floor_A_003.png";

void Renderer::init() {
    //WALLS
    if (!wallTexture.loadFromFile(WALL_PATH)) {
        std::cerr << "Erro ao carregar textura" << '\n';
    };

    if (wallTexture.getSize().x != wallTexture.getSize().y) {
        std::cerr << "ERRO: Textura não possui as mesmas dimensões " << '\n';
    }

    //FLOOR
    if (!floorImage.loadFromFile(FLOOR_PATH)) {
        std::cerr << "Erro ao carregar textura" << '\n';
    };

    if (floorImage.getSize().x != floorImage.getSize().y) {
        std::cerr << "ERRO: Textura não possui as mesmas dimensões " << '\n';
    }

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

    float radians = player.angle * PI/180.0f;
    sf::Vector2f direction{std::cos(radians), std::sin(radians)};
    sf::Vector2f plane {-direction.y, direction.x};
    sf::Vector2f position = player.position / map.getCellSize();

    sf::VertexArray walls{sf::Lines};

    static sf::Texture floorTextureGPU;
    static bool initialized = false;
    static std::vector<sf::Uint8> floorPixels((size_t)SCREEN_W * (size_t)SCREEN_H * 4);

    if (!initialized) {
        floorTextureGPU.create(SCREEN_W, SCREEN_H);
        initialized = true;
    }

    const sf::Uint8* floorData = floorImage.getPixelsPtr();
    int texSize = floorImage.getSize().x;
    int mask = texSize - 1;

    for (size_t y = SCREEN_H/2; y < SCREEN_H; y++) {
        sf::Vector2f rayDirLeft {direction - plane}, rayDirRight{direction + plane};
        float rowDistance = CAMERA_Z / ((float)y - SCREEN_H/2);
        sf::Vector2f floorStep = rowDistance * (rayDirRight - rayDirLeft)/SCREEN_W;
        sf::Vector2f floor = position + rowDistance * rayDirLeft;

        for (size_t x = 0; x < SCREEN_W; x++) {
            sf::Vector2i cell{floor};
            sf::Vector2i texCoords{(int)(texSize * (floor.x - cell.x)), (int)(texSize * (floor.y - cell.y))};
            texCoords.x &= mask;
            texCoords.y &= mask;

            int idx = (texCoords.y * texSize + texCoords.x) * 4;
            int dst = (x + y * (size_t)SCREEN_W) * 4;
            floorPixels[dst + 0] = floorData[idx + 0];
            floorPixels[dst + 1] = floorData[idx + 1];
            floorPixels[dst + 2] = floorData[idx + 2];
            floorPixels[dst + 3] = floorData[idx + 3];

            floor += floorStep;
        }
    }

    floorTextureGPU.update(floorPixels.data());
    sf::Sprite floorSprite{floorTextureGPU};
    target.draw(floorSprite);

    for (size_t i = 0; i < SCREEN_W; i++) {
        float cameraX = i * 2.0f / SCREEN_W - 1.0f;
        sf::Vector2f rayPos = position;
        sf::Vector2f rayDir = direction + plane * cameraX;

        sf::Vector2f deltaDist {
            std::abs(1.0f / rayDir.x),
            std::abs(1.0f / rayDir.y)
        };

        sf::Vector2i mapPos{rayPos};
        sf::Vector2i step;
        sf::Vector2f sideDist;

        if (rayDir.x < 0.0f) {
            step.x = -1;
            sideDist.x = (-mapPos.x + rayPos.x) * deltaDist.x;
        } else {
            step.x = 1;
            sideDist.x = (mapPos.x - rayPos.x + 1.0f) * deltaDist.x;
        }

        if (rayDir.y < 0.0f) {
            step.y = -1;
            sideDist.y = (-mapPos.y + rayPos.y) * deltaDist.y;
        } else {
            step.y = 1;
            sideDist.y = (mapPos.y - rayPos.y + 1.0f) * deltaDist.y;
        }

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

        if (didHit) {
            float perpWallDist = isHitVertical? sideDist.y - deltaDist.y : sideDist.x - deltaDist.x;
            float wallHeight = SCREEN_H / perpWallDist;
            float wallStart = (-wallHeight + SCREEN_H)/2.0f;
            float wallEnd = (wallHeight + SCREEN_H)/2.0f;

            float textureSize = wallTexture.getSize().x;
            float wallX = isHitVertical? rayPos.x + perpWallDist * rayDir.x :
                                         rayPos.y + perpWallDist * rayDir.y;
            wallX -= std::floor(wallX);
            float textureX = wallX * textureSize;

            float brightness = 1.0f - (perpWallDist/ (float)MAX_RAYCAST_DEPTH);
            if (isHitVertical) {
                brightness *= 0.7f;
            }

            sf::Color color = sf::Color(255*brightness, 255*brightness, 255*brightness);
            walls.append(sf::Vertex(sf::Vector2f((float)i, wallStart), color, sf::Vector2f(textureX, 0.0f)));
            walls.append(sf::Vertex(sf::Vector2f((float)i, wallEnd), color, sf::Vector2f(textureX, textureSize)));
        }
    }

    sf::RenderStates states{&wallTexture};
    target.draw(walls, states);
}



