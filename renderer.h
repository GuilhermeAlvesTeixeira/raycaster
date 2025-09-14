//
// Created by usuario on 14/09/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "player.h"
#include "map.h"

constexpr float SCREEN_W = 1200.0f;
constexpr float SCREEN_H = 675.0f;

class Renderer {
public:
    void draw3dView(sf::RenderTarget &target, const Player &player, const Map &map);
    void drawRays(sf::RenderTarget &target, const Player &player, const Map &map);
private:
};

#endif //RENDERER_H
